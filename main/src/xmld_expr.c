/*                                                                *
 * -------------------------------------------------------------- *
 * The OpenXMLD                                                   *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#include <limits.h>
#include <math.h>
#include "includes.h"

/*
 * : Creates a new expression.
 * returns: the newly created expression.
 */
XMLDExpr *XMLDExpr_create() {
 XMLDExpr *expr=(XMLDExpr *) malloc(sizeof(XMLDExpr));
 expr->aggr=XMLD_FALSE;
 expr->left=NULL;
 expr->right=NULL;
 expr->ident=NULL;
 expr->file=NULL;
 expr->func=NULL;
 expr->arg_list=NULL;
 expr->qval=NULL;
 expr->exprs=NULL;
 expr->alias=NULL;
 expr->cross_level=XMLD_FALSE;
 return expr;
}

/*
 * : Frees an already allocated expression.
 * Note: this function uses the type of the expression
 * to free only the releavant fields, it's the coder's responsibility
 * to free fields not included in the current type of the expression.
 */
void XMLDExpr_free(XMLDExpr *expr) {
 if (expr != NULL) {
  XMLDExpr_free_content((void *) expr);
  free(expr);
 } 
}

/*
 * : Frees the memory internally allocated by an expression.
 * vexpr: a void pointer the expression whose memory is to be freed.
 */
void XMLDExpr_free_content(void *expr) {
 switch(((XMLDExpr *)expr)->type) {
  case XMLD_OPERATION:
   XMLDExpr_free(((XMLDExpr *)expr)->left);
   XMLDExpr_free(((XMLDExpr *)expr)->right);
   break;
  case XMLD_IDENTIFIER:
   cfree(((XMLDExpr *)expr)->ident);
   break;
  case XMLD_FUNCTION:
   XMLDList_free(((XMLDExpr *)expr)->arg_list);
   break;
  case XMLD_QVAL:
   cfree(((XMLDExpr *)expr)->qval);
   break;
  case XMLD_LIST:
   XMLDList_free(((XMLDExpr *)expr)->exprs);
  break;
 }
 cfree(((XMLDExpr *)expr)->alias);
}

/*
 * : Copies an expression into another expression.
 */
void XMLDExpr_copy(XMLDExpr *src, XMLDExpr *dest) {
 dest->type=src->type;
 dest->cross_level=src->cross_level;
 switch (src->type) {
  case XMLD_INTEGER:
   dest->nval=src->nval;
  break;
  case XMLD_OPERATION:
   dest->left=src->left;
   dest->right=src->right;
   dest->op=src->op;
  break;
  case XMLD_IDENTIFIER:
   dest->ident=src->ident;
   dest->file=src->file;
  break;
  case XMLD_FUNCTION:
   dest->func=src->func;
   dest->arg_list=src->arg_list;
  break;
  case XMLD_QVAL:  
   dest->qval=src->qval;
  break;
  case XMLD_WILDCARD:
   dest->wildcard=src->wildcard;
  break;
  case XMLD_FLOAT:
   dest->fnval=src->fnval;
  break;
  case XMLD_LIST:
   dest->exprs=src->exprs;
  break;
 }
 dest->alias=src->alias;
}


/*
 * Tells whether the given expression needs processing
 * before being able to get a direct value out of it.
 */
XMLDBool XMLDExpr_is_complex(XMLDExpr *expr) {
 if (expr == NULL || expr->type == XMLD_INTEGER
     || expr->type == XMLD_QVAL || expr->type == XMLD_FLOAT) {
  return XMLD_FALSE;
 }
 else {
  return XMLD_TRUE;
 }
}

/*
 * Returns a simplified copy of the given expression
 * such that XMLDExpr_is_complex returns XMLD_FALSE if 
 * called with the returned expression as an argument.
 *
 * Exception: XMLD_LIST type is not simplified internally
 * but a caller must invoke this function on the elements
 * of the returned list.
 *
 * NULL is returned in case of error and xmld_errno is
 * set appropriately.
 */
XMLDExpr *XMLDExpr_simplify(XMLDExpr *expr, XMLDWork *work, int level) {
 XMLDExpr *ret;
 xmld_errno = XMLD_ENOERROR;
 if (expr->type == XMLD_OPERATION) {
  XMLDExpr *left, *right;
  if (XMLDExpr_is_complex(expr->left)) {
   left=XMLDExpr_simplify(expr->left, work, level);
  }
  else {
   left=expr->left;
  }
  
  if (left == NULL && xmld_errno != XMLD_ENOERROR) {
   return NULL;
  }
  
  xmld_errno = XMLD_ENOERROR;
  if (XMLDExpr_is_complex(expr->right)) {
   right=XMLDExpr_simplify(expr->right, work, level);
  }
  else {
   right=expr->right;
  }
  
  if (right == NULL && xmld_errno != XMLD_ENOERROR) {
   if (XMLDExpr_is_complex(expr->left)) {
    XMLDExpr_free(left);
   }
   return NULL;   
  }

  switch (expr->op) {
   case XMLD_OP_ADD:
    if (left->type == XMLD_QVAL || right->type == XMLD_QVAL) {
     xmld_errno = XMLD_ESTRADD;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
    else if (left->type == right->type) {
     switch(left->type) {
      case XMLD_INTEGER:
       ret=XMLDExpr_create();
       ret->type = XMLD_INTEGER;
       ret->nval=left->nval + right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=left->fnval + right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->fnval + right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->nval + right->fnval;
     }
    }
   break;
   case XMLD_OP_BNEG:
    if (left->type == XMLD_QVAL || right->type == XMLD_QVAL) {
     xmld_errno = XMLD_ESTRBNEG;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
    else if (left->type == right->type) {
     switch(left->type) {
      case XMLD_INTEGER:
       ret=XMLDExpr_create();
       ret->type = XMLD_INTEGER;
       ret->nval=left->nval - right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=left->fnval - right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->fnval - right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->nval - right->fnval;
     }
    }
   break;
   case XMLD_OP_MULTIP:
    if (left->type == XMLD_QVAL || right->type == XMLD_QVAL) {
     xmld_errno = XMLD_ESTRMULTIP;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
    else if (left->type == right->type) {
     switch(left->type) {
      case XMLD_INTEGER:
       ret=XMLDExpr_create();
       ret->type = XMLD_INTEGER;
       ret->nval=left->nval * right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=left->fnval * right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->fnval * right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->nval * right->fnval;
     }
    }
   break;
   case XMLD_OP_DIV:
    if (left->type == XMLD_QVAL || right->type == XMLD_QVAL) {
     xmld_errno = XMLD_ESTRDIV;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
    else if (left->type == right->type) {
     switch(left->type) {
      case XMLD_INTEGER:
       if (right->nval == 0) {
        xmld_errno = XMLD_EZERODIV;
        if (XMLDExpr_is_complex(expr->left)) {
         XMLDExpr_free(left);
        }
        if (XMLDExpr_is_complex(expr->right)) {
         XMLDExpr_free(right);
        }
        return NULL;
       }
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=left->nval / right->nval;
      break; 
      case XMLD_FLOAT:
       if (right->fnval == 0) {
        xmld_errno = XMLD_EZERODIV;
        if (XMLDExpr_is_complex(expr->left)) {
         XMLDExpr_free(left);
        }
        if (XMLDExpr_is_complex(expr->right)) {
         XMLDExpr_free(right);
        }
        return NULL;
       }
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=left->fnval / right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      if (right->nval == 0) {
       xmld_errno = XMLD_EZERODIV;
       if (XMLDExpr_is_complex(expr->left)) {
        XMLDExpr_free(left);
       }
       if (XMLDExpr_is_complex(expr->right)) {
        XMLDExpr_free(right);
       }
       return NULL;
      }
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->fnval / right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      if (right->fnval == 0) {
       xmld_errno = XMLD_EZERODIV;
       if (XMLDExpr_is_complex(expr->left)) {
        XMLDExpr_free(left);
       }
       if (XMLDExpr_is_complex(expr->right)) {
        XMLDExpr_free(right);
       }
       return NULL;
      }
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=left->nval / right->fnval;
     }
    }
   break;
   case XMLD_OP_EXPO:
    if (left->type == XMLD_QVAL || right->type == XMLD_QVAL) {
     xmld_errno = XMLD_ESTREXPO;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
    else if (left->type == right->type) {
     switch(left->type) {
      case XMLD_INTEGER:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=pow(left->nval, right->nval);
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->fnval=pow(left->fnval, right->fnval);
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=pow(left->fnval, right->nval);
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=pow(left->nval, right->fnval);
     }
    }
   break;
   case XMLD_OP_UNEG:
    switch (right->type) {
     case XMLD_QVAL:
      xmld_errno = XMLD_ESTRUNEG;
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     break;
     case XMLD_INTEGER:
      ret=XMLDExpr_create();
      ret->type=XMLD_INTEGER;
      ret->nval=-right->nval;
     break;
     case XMLD_FLOAT:
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->fnval=-right->fnval;
     break;
    } 
   break;
  }
  
  if (XMLDExpr_is_complex(expr->left)) {
   XMLDExpr_free(left);
  }
  if (XMLDExpr_is_complex(expr->right)) {
   XMLDExpr_free(right);
  }	  
 }
 else if (expr->type == XMLD_IDENTIFIER) {
  if (expr->file == NULL) {
   expr->file = (XMLDFile *) XMLDList_first(work->files);
  }
  else if (expr->file->data == NULL) {
   XMLDFile *curr_file=expr->file;
   expr->file = XMLDFileList_search_by_name(work->files, expr->file->name);
   XMLDFile_free(curr_file);
  }
  
  if (level != 0 && expr->file->level != level) {
   ret = XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=NULL;
   return ret;
  }
  /* -- you have stopped here -- */  
  if (expr->file->store == NULL) {
   ret=XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
   return ret;
  }
  
  fpos_t pos;
  fgetpos((FILE *) expr->file->data, &pos);
  char *tagname=engine_xmld_get_tagname((FILE *) expr->file->data);
  fsetpos((FILE *) expr->file->data, &pos);
  char *type=engine_xmld_get_element_att_type((FILE *) expr->file->store, level, tagname, expr->ident);
  free(tagname);
  if (type == NULL) {
   ret=XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
  }
  else if (strcasecmp(type, XMLD_TYPE_CHAR) == 0) {
   ret=XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
  }
  else if (strcasecmp(type, XMLD_TYPE_INT) == 0) {
   ret=XMLDExpr_create();
   ret->type = XMLD_INTEGER;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
   ret->nval=atoi(ret->qval);
   free(ret->qval);
   ret->qval=NULL;
  }
  else if (strcasecmp(type, XMLD_TYPE_FLOAT) == 0) {
   ret=XMLDExpr_create();
   ret->type = XMLD_FLOAT;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
   ret->fnval=atof(ret->qval);
   free(ret->qval);
   ret->qval=NULL;
  }
  else {
   ret=XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=engine_xmld_get_column_value(expr->file, expr->ident);
  }
  free(type);
 }
 else if (expr->type == XMLD_FUNCTION) {
  ret=(*(expr->func->func)) (expr->arg_list, expr->file);
 }
 else if (expr->type == XMLD_WILDCARD) {
  if (expr->file == NULL) {
   expr->file = (XMLDFile *) XMLDList_first(work->files);
  }
  ret=XMLDExpr_create();
  ret->type=XMLD_QVAL;
  ret->qval=engine_xmld_get_column_value(expr->file, (expr->wildcard == XMLD_WILDCARD_ALL) ? "*" : "@");
 }
 return ret;
}

/*
 * : Create a list of expressions.
 * returns: the newly created expression list.
 */
XMLDExprList *XMLDExprList_create() {
 XMLDExprList *list=XMLDList_create(sizeof(XMLDExpr), XMLDExpr_free_content);
 return list;
}

/*
 * : Adds a new element to a list of expressions.
 * list: the list to which the element is to be added.
 * returns: a pointer to the newly added element.
 */ 
XMLDExpr *XMLDExprList_add(XMLDExprList *list) {
 XMLDExpr *expr=(XMLDExpr *) XMLDList_add(list);
 expr->aggr=XMLD_FALSE;
 expr->left=NULL;
 expr->right=NULL;
 expr->ident=NULL;
 expr->file=NULL;
 expr->func=NULL;
 expr->arg_list=NULL;
 expr->qval=NULL;
 expr->exprs=NULL;
 expr->alias=NULL;
 expr->cross_level=XMLD_FALSE;
 return expr;
}
