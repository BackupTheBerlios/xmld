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
  case XMLD_SPECIAL_IDENTIFIER:
   dest->sident=src->sident;
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
 * Read the given type and transform the given no-type string 
 * expression to it.
 */ 
void XMLDExpr_apply_type(XMLDExpr *expr, char *type) {
 if (type == NULL || strcasecmp(type, XMLD_TYPE_CHAR) == 0) {
  expr->type = XMLD_QVAL;
 }
 else if (strcasecmp(type, XMLD_TYPE_INT) == 0) {
  expr->type = XMLD_INTEGER;
  expr->nval=atoi(expr->qval);
  if (expr->qval != NULL) {
   free(expr->qval);
   expr->qval=NULL;
  } 
 }
 else if (strcasecmp(type, XMLD_TYPE_FLOAT) == 0) {
  expr->type = XMLD_FLOAT;
  expr->fnval=atof(expr->qval);
  if (expr->qval != NULL) {
   free(expr->qval);
   expr->qval=NULL;
  } 
 }
 else {
  expr->type = XMLD_QVAL;
 }
}

/*
 * Returns a string representation of the given simple 
 * expression.
 */
char *XMLDExpr_to_string(XMLDExpr *expr) {
 char *ret;
 if (expr->type == XMLD_INTEGER) {
  return itostr(expr->nval, 0);
 } 
 else if (expr->type == XMLD_QVAL) {
  if (expr->qval == NULL) {
   return NULL;
  }
  ret=(char *) malloc((strlen(expr->qval)+1)*sizeof(char));
  strcpy(ret, expr->qval);
  return ret;
 }
 else if (expr->type == XMLD_FLOAT) {
  return ftostr(expr->fnval, 0);
 }
 else {
  return NULL;
 }  
}

/*
 * Returns the boolean representation of the given expression.
 */
XMLDBool XMLDExpr_to_boolean(XMLDExpr *expr) {
 if (expr->type == XMLD_INTEGER) {
  if (expr->nval == 0) {
   return XMLD_FALSE;
  }
  else {
   return XMLD_TRUE;
  }
 }
 else if (expr->type == XMLD_FLOAT) {
  if (expr->fnval == 0) {
   return XMLD_FALSE;
  }
  else {
   return XMLD_TRUE;
  }
 }
 else if (expr->type == XMLD_QVAL) {
  if (expr->qval == NULL || strcmp(expr->qval, "") == 0) {
   return XMLD_FALSE;
  }
  else {
   return XMLD_TRUE;
  }
 }
 else {
  return XMLD_FALSE;
 }
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
 * Fills the response structure of the given work structure with the simplest 
 * form(s) of the given expression.
 */
XMLDStatus XMLDExpr_to_columns(XMLDExpr *expr, XMLDWork *work, int level) {
 if (expr->type == XMLD_LIST) {
  XMLDList_reset(expr->exprs);
  while (XMLDList_next(expr->exprs)) {
   if (XMLDExpr_to_columns((XMLDExpr *) XMLDList_curr(expr->exprs), work) == XMLD_FAILURE) {
    return XMLD_FAILURE;
   }
  }
 }
 else {
  XMLDExpr *simple = twalker_simplify_expr(expr, work, level);
  if (simple == NULL) {
   return XMLD_FAILURE;
  }
  else {
   XMLDResponse_add_col(work->resp);
   XMLDResponse_fill_col(simple);
   return XMLD_SUCCESS;
  }
 }
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
