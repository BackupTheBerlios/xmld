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

#include <stdlib.h>
#include <string.h>
#include "xmlddef.h"
#include "xmld_list.h"
#include "xmld_col.h"
#include "xmld_row.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_aggr_table.h"
#include "xmld_cond.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_response.h"
#include "xmld_request.h"
#include "xmld_connection.h"
#include "xmld_file.h"
#include "xmld_work.h"
#include "xmld_engine.h"
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
   
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    char *full_file=XMLDFile_get_full_name(curr_file, work);
    int priv=authman_get_priv(work->conn->user, full_file);
   
    if (!BIT_ISSET(priv, XMLD_PRIV_READ)) {
     free(full_file);
     xmld_errno=XMLD_ENORPRIV;
     return XMLD_FAILURE;
    }
   
    curr_file->engine=XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
    free(full_file);
   
    if (curr_file->engine == NULL) {
     xmld_errno=XMLD_ENOENGINE;
     return XMLD_FAILURE;
    }

    if (((*(curr_file->engine->prepare)) (work, curr_file, XMLD_ACCESS_NOTHING)) == XMLD_FAILURE) {
     XMLDList_reset(work->files) 
     while (XMLDList_next(work->files)) {
      XMLDFile *cur_file=(XMLDFile *) XMLDList_curr(work->files);
      if (cur_file == curr_file) {
       break;
      }
      (*(cur_file->engine->cleanup)) (work, cur_file);
     }
     return XMLD_FAILURE;
    }
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
    ret=(*(curr_file->engine->walk)) (work, curr_file);
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
      /* Handle expressions of type "expression list" */
      if (((XMLDExpr *) XMLDList_curr(work->req->retr))->type == XMLD_LIST) {
       XMLDExprList *expr_list=((XMLDExpr *) XMLDList_curr(work->req->retr))->exprs;
       XMLDList_reset(expr_list);
       while (XMLDList_next(expr_list)) {
        XMLDResponse_add_col(work->resp);
        if (((XMLExpr *) XMLDList_curr(expr_list))->aggr == XMLD_TRUE) {
         XMLDAggrTable *curr_table=XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(expr_list), XMLDResponse_curr_col(work->resp));
         (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(expr_list), curr_table, curr_max_level);
        }
        else {
         XMLDResponse_fill_col(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(expr_list), NULL, curr_max_level));
        } 
       }
      }
      else {	      
       XMLDResponse_add_col(work->resp);
       if (((XMLExpr *) XMLDList_curr(work->req->retr))->aggr == XMLD_TRUE) {
        XMLDAggrTable *curr_table=XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(work->req->retr), XMLDResponse_curr_col(work->resp));
        (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr), curr_table, curr_max_level);
       }
       else {
        XMLDResponse_fill_col(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr), NULL, curr_max_level));
       } 
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
      ret=(*(work->res->engine->walk)) (work, curr_file);
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
     curr_max_level == XMLDFileList_get_max_level(work->files);
    }
   }
   
   /* Filling columns associated to aggregate expressions with values */
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_next_aggr(work->resp)) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDResponse_curr_aggr_table(work->resp))->value, NULL, curr_max_level));
   }
   
   /* Handling of level changes not followed by row retrieval */
   XMLDRow *last_row=XMLDResponse_curr_row(work->resp);
   if (last_row != NULL) {
    last_row->num_up += num_up;
    last_row->num_down += num_down;
   }

   char *resp=resptrans_handle(work);
   if (protoimpl_write_sequence(work->conn->fd, resp, 1) == XMLD_FAILURE) {
    free(resp);
    return XMLD_FAILURE;
   }
   free(resp);
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    XMLDFile *curr_file=(XMLDFile *) XMLDList_curr(work->files);
    (*(curr_file->engine->cleanup)) (work, curr_file);
   }	   
  break;
  case XMLD_SQL_SELECT_WHERE:
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    curr_file=(XMLDFile *) XMLDList_curr(work->files);
    full_file=XMLDFile_get_full_name(curr_file, work);
    priv=authman_get_priv(work->conn->user, full_file);
   
    if (!BIT_ISSET(priv, XMLD_PRIV_READ)) {
     free(full_file);
     xmld_errno=XMLD_ENORPRIV;
     return XMLD_FAILURE;
    }
   
    curr_file->engine=XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
    free(full_file);
   
    if (curr_file->engine == NULL) {
     xmld_errno=XMLD_ENOENGINE;
     return XMLD_FAILURE;
    }
    
    if (((*(curr_file->engine->prepare)) (work, curr_file, XMLD_ACCESS_FORMAT)) == XMLD_FAILURE) {
     XMLDList_reset(work->files);
     while (XMLDList_next(work->files)) {
      XMLDFile *cur_file=(XMLDFile *) XMLDList_curr(work->files);
      if (cur_file == curr_file) {
       break;
      }
      (*(cur_file->engine->cleanup)) (work, cur_file);
     }
     return XMLD_FAILURE;
    }
   }

   work->resp=XMLDResponse_create();
   XMLDList_reset(work->req->where);
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
    ret=(*(curr_file->engine->walk)) (work, curr_file);
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
      retr=XMLD_TRUE;
     }
     where=XMLDList_next(work->req->where);
     if (((XMLDCond *) XMLDList_curr(work->req->where))->cross_level == XMLD_TRUE) {
      where = XMLD_TRUE;
     }
     retr = retr && where;
     if (retr && !(((XMLDExpr *) XMLDList_curr(work->req->retr))->type == XMLD_VOID_LIST)) {
      if ((*(work->res->engine->eval_cond)) (work, (XMLDCond *) XMLDList_curr(work->req->where), curr_max_level)) {
       XMLDResponse_add_row(work->resp);
       XMLDResponse_curr_row(work->resp)->num_down=num_down;
       XMLDResponse_curr_row(work->resp)->num_up=num_up;
       num_up=num_down=0;
       /* Handle expressions of type "expression list" */
       if (((XMLDExpr *) XMLDList_curr(work->req->retr))->type == XMLD_LIST) {
        XMLDExprList *expr_list=((XMLDExpr *) XMLDList_curr(work->req->retr))->exprs;
        XMLDList_reset(expr_list);
        while (XMLDList_next(expr_list)) {
	 XMLDResponse_add_col(work->resp);
	 if (((XMLExpr *) XMLDList_curr(expr_list))->aggr == XMLD_TRUE) {
	  XMLDAggrTable *curr_table=XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(expr_list), XMLDResponse_curr_col(work->resp));
	  (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(expr_list), curr_table, curr_max_level);
	 }
	 else {
          XMLDResponse_fill_col(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(expr_list), NULL, curr_max_level));
	 } 
        }
       }
       else {
	XMLDResponse_add_col(work->resp);
	if (((XMLExpr *) XMLDList_curr(work->req->retr))->aggr == XMLD_TRUE) {
	 XMLDAggrTable *curr_table=XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(work->req->retr), XMLDResponse_curr_col(work->resp));
	 (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr), curr_table, curr_max_level);
	}
	else {
         XMLDResponse_fill_col(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr), NULL, curr_max_level));
	} 
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
           ret=(*(work->res->engine->walk)) (work, curr_file);
           else if (ret == XMLD_WALK_DOWN) {
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
      ret=(*(work->res->engine->walk)) (work, curr_file);
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
     curr_max_level == XMLDFileList_get_max_level(work->files);
    }
   }
   
   /* Filling columns associated to aggregate expressions with values */
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_next_aggr(work->resp)) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, XMLDResponse_curr_aggr_table(work->resp)->value, NULL, curr_max_level));
   }

   /* Handling of level changes not followed by row retrieval */
   last_row=XMLDResponse_curr_row(work->resp);
   if (last_row != NULL) {
    last_row->num_up += num_up;
    last_row->num_down += num_down;
   }

   resp=resptrans_handle(work);
   if (protoimpl_write_sequence(work->conn->fd, resp, 1) == XMLD_FAILURE) {
    free(resp);
    return XMLD_FAILURE;
   }
   free(resp);
   XMLDList_reset(work->files);
   while (XMLDList_next(work->files)) {
    XMLDFile *curr_file=(XMLDFile *) XMLDList_curr(work->files);
    (*(curr_file->engine->cleanup)) (work, curr_file);
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
