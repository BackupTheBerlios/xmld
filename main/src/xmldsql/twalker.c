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

#include "includes.h"
#include "cfg.h"
#include "engine_list.h"
#include "twalker.h"
#include "xmld-sql.h"
#include "resptrans.h"
#include "protoimpl.h" 
#include "authman.h"
 
XMLDStatus twalker_handle(XMLDWork *work) {
 switch(work->req->type) {
  case XMLD_SQL_SELECT:
   XMLDList_reset(work->files);
   XMLDFile *curr_file;
   char *full_file;
   
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    full_file=XMLDFile_get_full_name(curr_file, work);
    curr_file->priv=authman_get_priv(work->conn->user, full_file);
    curr_file->engine=XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
   
    if (curr_file->engine == NULL) {
     xmld_errno=XMLD_ENOENGINE;
     return XMLD_FAILURE;
    }

    if ((*(curr_file->engine->prepare) (full_file, curr_file, XMLD_ACCESS_FORMAT)) == XMLD_FAILURE) {
     free(full_file);
     XMLDList_reset(work->files);
     while (XMLDList_next(work->files)) {
      XMLDFile *cur_file=(XMLDFile *) XMLDList_curr(work->files);
      if (cur_file == curr_file) {
       break;
      }
      *(cur_file->engine->cleanup) (work, cur_file);
     }
     return XMLD_FAILURE;
    }
    free(full_file);
   }
   
   work->resp=XMLDResponse_create();
   XMLDList_reset(work->req->retr);
   int ret=0;
   int curr_max_level=1;
   XMLDBool retr=XMLD_TRUE;
   int num_up, num_down, ret_all=XMLD_WALK_DOWN;
   num_up=num_down=0;
   
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    ret=*(curr_file->engine->walk) (curr_file);
    if (ret == XMLD_WALK_END) {
     ret_all=XMLD_WALK_END;
     break;
    }
   }
   
   while (ret_all != XMLD_WALK_END) {
    if (ret_all == XMLD_WALK_DOWN) {
     num_down++;
     retr=XMLDList_next(work->req->retr);
     if (((XMLDExpr *) XMLDList_curr(work->req->retr))->cross_level == XMLD_TRUE) {
      retr=XMLD_TRUE;
     }
     if (retr && !(((XMLDExpr *) XMLDList_curr(work->req->retr))->type == XMLD_VOID_LIST)) {
      XMLDResponse_add_row(work->resp);
      XMLDResponse_curr_row(work->resp)->num_down=num_down;
      XMLDResponse_curr_row(work->resp)->num_up=num_up;
      num_up=num_down=0;
      if (XMLDExpr_to_columns((XMLDExpr *) XMLDList_curr(work->req->retr), work, curr_max_level) == XMLD_FAILURE) {
       return XMLD_FAILURE; 
      }
     } 
    }
    else {
     num_up++;
     XMLDList_prev(work->req->retr);
    }
    XMLDList_reset(work->files);
    ret_all=XMLD_WALK_UP;
    while (XMLDList_next(work->files)) {
     curr_file=(XMLDFile *) XMLDList_curr(work->files);
     if (curr_file->level == curr_max_level) {
      ret=*(curr_file->engine->walk) (curr_file);
      if (ret == XMLD_WALK_END) {
       ret_all=XMLD_WALK_END;
       break;
      }
      else if (ret == XMLD_WALK_DOWN) {
       ret_all=XMLD_WALK_DOWN;
      }
     } 
    }
    if (ret_all != XMLD_WALK_END) {
     curr_max_level = XMLDFileList_get_max_level(work->files);
    }
   }
   
   /* Handling of level changes not followed by row retrieval */
   XMLDRow *last_row=XMLDResponse_curr_row(work->resp);
   if (last_row != NULL) {
    last_row->num_up += num_up;
    last_row->num_down += num_down;
   }
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    XMLDFile *curr_file=(XMLDFile *) XMLDList_curr(work->files);
    *(curr_file->engine->cleanup) (curr_file);
   }
  break;
  case XMLD_SQL_SELECT_WHERE:
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    full_file=XMLDFile_get_full_name(curr_file, work);
    curr_file->priv=authman_get_priv(work->conn->user, full_file);
    curr_file->engine=XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
   
    if (curr_file->engine == NULL) {
     xmld_errno=XMLD_ENOENGINE;
     return XMLD_FAILURE;
    }
    
    if ((*(curr_file->engine->prepare) (full_file, curr_file, XMLD_ACCESS_FORMAT)) == XMLD_FAILURE) {
     free(full_file);
     XMLDList_reset(work->files);
     while (XMLDList_next(work->files)) {
      XMLDFile *cur_file=(XMLDFile *) XMLDList_curr(work->files);
      if (cur_file == curr_file) {
       break;
      }
      *(cur_file->engine->cleanup) (work, cur_file);
     }
     return XMLD_FAILURE;
    }
    free(full_file);
   }

   work->resp=XMLDResponse_create();
   XMLDList_reset(work->req->where);
   XMLDList_reset(work->req->retr);
   ret=0;
   ret_all=XMLD_WALK_DOWN;
   retr=XMLD_TRUE;
   XMLDBool where=XMLD_TRUE;
   num_up=num_down=0;
   curr_max_level=1;
   int curr_level=0;
   
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    ret=*(curr_file->engine->walk) (curr_file);
    if (ret == XMLD_WALK_END) {
     ret_all=XMLD_WALK_END;
     break;
    }
   }
   
   while (ret_all != XMLD_WALK_END) {
    if (ret_all == XMLD_WALK_DOWN) {
     curr_level++;
     num_down++;
     retr=XMLDList_next(work->req->retr);
     if (((XMLDExpr *) XMLDList_curr(work->req->retr))->cross_level == XMLD_TRUE) {
      retr = XMLD_TRUE;
     }
     where=XMLDList_next(work->req->where);
     if (((XMLDCond *) XMLDList_curr(work->req->where))->cross_level == XMLD_TRUE) {
      where = XMLD_TRUE;
     }
     retr = retr && where;
     if (retr && !(((XMLDExpr *) XMLDList_curr(work->req->retr))->type == XMLD_VOID_LIST)) {
      XMLDExpr *condition = twalker_simplify_expr((XMLDExpr *) XMLDList_curr(work->req->where), work, curr_max_level);
      retr = XMLDExpr_to_boolean(condition);
      XMLDExpr_free(condition);
      if (retr == XMLD_TRUE) {
       XMLDResponse_add_row(work->resp);
       XMLDResponse_curr_row(work->resp)->num_down=num_down;
       XMLDResponse_curr_row(work->resp)->num_up=num_up;
       num_up=num_down=0;
       if (XMLDExpr_to_columns((XMLDExpr *) XMLDList_curr(work->req->retr), work, curr_max_level) == XMLD_FAILURE) {
        return XMLD_FAILURE;
       }
      }
      else {
       if (((XMLDCond *) XMLDList_curr(work->req->where))->cross_level != XMLD_TRUE) {
        int tmp_level=curr_level;
        do {
         XMLDList_reset(work->files);
         ret_all=XMLD_WALK_UP;
         while (XMLDList_next(work->files)) {
          curr_file=(XMLDFile *) XMLDList_curr(work->files);
          if (curr_file->level == curr_max_level) {
           ret=*(curr_file->engine->walk) (curr_file);
           if (ret == XMLD_WALK_DOWN) {
            ret_all=XMLD_WALK_DOWN;
           }
          } 
         }
         curr_max_level == XMLDFileList_get_max_level(work->files);
  	 if (ret == XMLD_WALK_DOWN) {
	  XMLDList_next(work->req->where);
	  XMLDList_next(work->req->retr);
	  curr_level++;
	 }
	 if (ret == XMLD_WALK_UP) {
	  XMLDList_prev(work->req->where);
	  XMLDList_prev(work->req->retr);
	  if (--curr_level == tmp_level - 1) {
	   break;
	  }
	 }
        } while (ret != XMLD_WALK_END);
        num_up++;
       }
      }
     }
    }
    else {
     num_up++;
     curr_level--;
     XMLDList_prev(work->req->retr);
     XMLDList_prev(work->req->where);
    }
    XMLDList_reset(work->files);
    ret_all=XMLD_WALK_UP;
    while (XMLDList_next(work->files)) {
     curr_file=(XMLDFile *) XMLDList_curr(work->files);
     if (curr_file->level == curr_max_level) {
      ret=*(curr_file->engine->walk) (curr_file);
      if (ret == XMLD_WALK_END) {
       ret_all=XMLD_WALK_END;
       break;
      }
      else if (ret == XMLD_WALK_DOWN) {
       ret_all=XMLD_WALK_DOWN;
      }
     } 
    }
    if (ret_all != XMLD_WALK_END) {
     curr_max_level = XMLDFileList_get_max_level(work->files);
    }
   }
   
   /* Handling of level changes not followed by row retrieval */
   last_row=XMLDResponse_curr_row(work->resp);
   if (last_row != NULL) {
    last_row->num_up += num_up;
    last_row->num_down += num_down;
   }
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    XMLDFile *curr_file=(XMLDFile *) XMLDList_curr(work->files);
    *(curr_file->engine->cleanup) (curr_file);
   }
  break;
  case XMLD_SQL_UPDATE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_UPDATE_WHERE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_DELETE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_DELETE_WHERE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_INSERT_COL:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_INSERT_COL_WHERE :
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_INSERT_WHERE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_INSERT:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
  case XMLD_SQL_USE:
   xmld_errno=XMLD_ENOTIMPL;
   return XMLD_FAILURE;
  break;
 }
 return XMLD_SUCCESS;
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
XMLDExpr *twalker_simplify_expr(XMLDExpr *expr, XMLDWork *work, int level) {
 XMLDExpr *ret;
 if (expr->type == XMLD_IDENTIFIER || expr->type == XMLD_SPECIAL_IDENTIFIER
     || expr->type == XMLD_WILDCARD) {
  if (expr->file == NULL) {
   expr->file = (XMLDFile *) XMLDList_first(work->files);
  }
  else if (expr->file->data == NULL) {
   XMLDFile *curr_file=expr->file;
   expr->file = XMLDFileList_search_by_name(work->files, expr->file->name);
   XMLDFile_free(curr_file);
  }
  if (!BIT_ISSET(expr->file->priv, XMLD_PRIV_READ)) {
   xmld_errno = XMLD_ENORPRIV;
   return NULL;
  }
  if (level != 0 && expr->file->level != level) {
   ret = XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->qval=NULL;
   return ret;
  }
 }

 xmld_errno = XMLD_ENOERROR;
 if (expr->type == XMLD_OPERATION) {
  XMLDExpr *left, *right;
  if (XMLDExpr_is_complex(expr->left)) {
   left=twalker_simplify_expr(expr->left, work, level);
  }
  else {
   left=expr->left;
  }
  
  if (left == NULL && xmld_errno != XMLD_ENOERROR) {
   return NULL;
  }
  
  xmld_errno = XMLD_ENOERROR;
  if (XMLDExpr_is_complex(expr->right)) {
   right=twalker_simplify_expr(expr->right, work, level);
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
       ret->ident = XMLDExpr_to_string(expr);
       ret->nval=left->nval + right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->ident = XMLDExpr_to_string(expr);
       ret->fnval=left->fnval + right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
      ret->fnval=left->fnval + right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
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
       ret->ident = XMLDExpr_to_string(expr);
       ret->nval=left->nval - right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->ident = XMLDExpr_to_string(expr);
       ret->fnval=left->fnval - right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
      ret->fnval=left->fnval - right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
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
       ret->ident = XMLDExpr_to_string(expr);
       ret->nval=left->nval * right->nval;
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->ident = XMLDExpr_to_string(expr);
       ret->fnval=left->fnval * right->fnval;
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
      ret->fnval=left->fnval * right->nval;
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
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
       ret->ident = XMLDExpr_to_string(expr);
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
       ret->ident = XMLDExpr_to_string(expr);
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
      ret->ident = XMLDExpr_to_string(expr);
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
      ret->ident = XMLDExpr_to_string(expr);
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
       ret->ident = XMLDExpr_to_string(expr);
       ret->fnval=pow(left->nval, right->nval);
      break; 
      case XMLD_FLOAT:
       ret=XMLDExpr_create();
       ret->type = XMLD_FLOAT;
       ret->ident = XMLDExpr_to_string(expr);
       ret->fnval=pow(left->fnval, right->fnval);
      break;
     }
    }
    else {
     if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
      ret->fnval=pow(left->fnval, right->nval);
     }
     else if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
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
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval=-right->nval;
     break;
     case XMLD_FLOAT:
      ret=XMLDExpr_create();
      ret->type=XMLD_FLOAT;
      ret->ident = XMLDExpr_to_string(expr);
      ret->fnval=-right->fnval;
     break;
    } 
   break;
   case XMLD_OP_EQUAL:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->type = XMLD_INTEGER;
     ret->ident = XMLDExpr_to_string(expr);
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval == right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval == right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) == 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      re->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval == right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval == right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_L:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->type = XMLD_INTEGER;
     ret->ident = XMLDExpr_to_string(expr);
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval < right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval < right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) < 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval < right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval < right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_G:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->ident = XMLDExpr_to_string(expr);
     ret->type = XMLD_INTEGER;
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval > right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval > right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) > 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval > right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval > right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_NE:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->ident = XMLDExpr_to_string(expr);
     ret->type = XMLD_INTEGER;
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval != right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval != right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) != 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval != right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval != right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_LE:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->ident = XMLDExpr_to_string(expr);
     ret->type = XMLD_INTEGER;
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval <= right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval <= right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) <= 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval <= right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval <= right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_GE:
    if (left->type == right->type) {
     ret = XMLDExpr_create();
     ret->ident = XMLDExpr_to_string(expr);
     ret->type = XMLD_INTEGER;
     switch(left->type) {
      case XMLD_INTEGER:
       ret->nval = (left->nval >= right->nval);
      break;
      case XMLD_FLOAT:
       ret->nval = (left->fnval >= right->fnval);
      break;
      case XMLD_QVAL:
       ret->nval = (strcmp(left->qval, right->qval) >= 0);
      break;
     }
    }
    else {
     if (left->type == XMLD_INTEGER && right->type == XMLD_FLOAT) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->nval >= right->fnval);
     }
     else if (left->type == XMLD_FLOAT && right->type == XMLD_INTEGER) {
      ret = XMLDExpr_create();
      ret->type = XMLD_INTEGER;
      ret->ident = XMLDExpr_to_string(expr);
      ret->nval = (left->fnval >= right->nval);
     }
     else {
      xmld_errno = XMLD_ESTRNUMCMP;
      if (XMLDExpr_is_complex(expr->left)) {
       XMLDExpr_free(left);
      }
      if (XMLDExpr_is_complex(expr->right)) {
       XMLDExpr_free(right);
      }
      return NULL;
     }
    }
   break;
   case XMLD_OP_LIKE:
    if (left->type == XMLD_QVAL && right->type = XMLD_QVAL) {
     ret = XMLDExpr_create();
     ret->type = XMLD_INTEGER;
     ret->ident = XMLDExpr_to_string(expr);
     ret->nval = str_like(left->qval, right->qval);
    }
    else {
     xmld_errno = XMLD_ENUMLIKE;
     if (XMLDExpr_is_complex(expr->left)) {
      XMLDExpr_free(left);
     }
     if (XMLDExpr_is_complex(expr->right)) {
      XMLDExpr_free(right);
     }
     return NULL;
    }
   break;
   case XMLD_OP_AND:
    ret = XMLDExpr_create();
    ret->type = XMLD_INTEGER;
    ret->ident = XMLDExpr_to_string(expr);
    ret->nval = (XMLDExpr_to_boolean(left) && XMLDExpr_to_boolean(right));
   break;
   case XMLD_OP_OR:
    ret = XMLDExpr_create();
    ret->type = XMLD_INTEGER;
    ret->ident = XMLDExpr_to_string(expr);
    ret->nval = (XMLDExpr_to_boolean(left) || XMLDExpr_to_boolean(right));
   break;
   case XMLD_OP_NOT:
    ret = XMLDExpr_create();
    ret->type = XMLD_INTEGER;
    ret->ident = XMLDExpr_to_string(expr);
    ret->nval = (!XMLDExpr_to_boolean(right));
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
  char *type = (*(expr->file->engine->get_attribute_type)) (expr->file, expr->ident);
  ret = XMLDExpr_create();
  ret->ident = XMLDExpr_to_string(expr);
  ret->qval=(*(expr->file->engine->get_attribute)) (expr->file, expr->ident);
  XMLDExpr_apply_type(ret, type);
  free(type);
 }
 else if (expr->type == XMLD_SPECIAL_IDENTIFIER) {
  if (expr->sident == XMLD_SIDENT_TEXT) {
   char *type = (*(expr->file->engine->get_text_type)) (expr->file);
   ret = XMLDExpr_create();
   ret->ident = XMLDExpr_to_string(expr);
   ret->qval=(*(expr->file->engine->get_text)) (expr->file);
   XMLDExpr_apply_type(ret, type);
   free(type);
  }
  else if (expr->sident == XMLD_SIDENT_TAGNAME) {
   ret = XMLDExpr_create();
   ret->type = XMLD_QVAL;
   ret->ident = XMLDExpr_to_string(expr);
   ret->qval=(*(expr->file->engine->get_tagname)) (expr->file);
  }
 }
 else if (expr->type == XMLD_FUNCTION) {
   ret=(*(expr->func->func)) (expr->arg_list, expr->file);
   ret->ident = XMLDExpr_to_string(expr);
 }
 else if (expr->type == XMLD_WILDCARD) {
  if (expr->wildcard == XMLD_WILDCARD_ALL) {
   XMLDExpr *tmp = XMLDExpr_create();
   XMLDExpr_copy(expr, tmp);
   tmp->wildcard = XMLD_WILDCARD_ATTS;
   ret = twalker_simplify_expr(tmp, work, level);
   ret = XMLDExpr_to_string(expr);
   free(tmp);
   XMLDExpr *text = XMLDExpr_create();
   text->type = XMLD_SPECIAL_IDENTIFIER;
   text->sident = XMLD_SIDENT_TEXT;
   tmp = twalker_simplify_expr(text, work, level);
   XMLDExpr_free(text);
   text = XMLDExprList_add(ret->exprs);
   XMLDExpr_copy(tmp, text);
   free(tmp);
  }
  else if (expr->wildcard == XMLD_WILDCARD_ATTS) {
   ret = XMLDExpr_create();
   ret->type = XMLD_LIST;
   ret->ident = XMLDExpr_to_string(expr);
   ret->exprs = XMLDExprList_create();   
   XMLDExpr *tmp;
   (*(expr->file->engine->reset_element)) (expr->file);
   while ((*(expr->file->engine->next_attribute)) (expr->file) == XMLD_TRUE) {
    char *type = (*(expr->file->engine->get_curr_attribute_type)) (expr->file);
    tmp = XMLDExprList_add(ret->exprs);
    tmp->ident = (*(expr->file->engine->get_curr_attribute_name)) (expr->file);
    tmp->qval = (*(expr->file->engine->get_curr_attribute_value)) (expr->file);
    XMLDExpr_apply_type(tmp, type);
    free(type);
   }
  }
 }
 return ret;
}
