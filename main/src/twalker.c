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
#include "xmld_sockets.h"
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
#include "xmld_resource.h"
#include "xmld_response.h"
#include "xmld_request.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"
#include "cfg.h"
#include "engine_list.h"
#include "twalker.h"
#include "xmld-sql.h"
#include "resptrans.h"
#include "authman.h"
 
XMLDStatus twalker_handle(XMLDWork *work) {
 switch(work->req->type) {
  case XMLD_SQL_SELECT:
   work->res=XMLDResource_create();
  
   char *full_file=XMLDWork_get_full_file(work);
   int priv=authman_get_priv(work->conn->user, full_file);
   
   if (!BIT_ISSET(priv, XMLD_PRIV_READ)) {
    free(full_file);
    xmld_errno=XMLD_ENORPRIV;
    return XMLD_FAILURE;
   }
   
   work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
   free(full_file);
   
   if (work->res->engine == NULL) {
    xmld_errno=XMLD_ENOENGINE;
    return XMLD_FAILURE;
   }
   
   if (((*(work->res->engine->prepare)) (work, 0)) == XMLD_FAILURE) {
    return XMLD_FAILURE;
   }

   work->resp=XMLDResponse_create();
   int ret=(*(work->res->engine->walk)) (work);
   while (ret != XMLD_WALK_END) {
    if (ret == XMLD_WALK_DOWN) {
     XMLDList_reset(work->req->retr);
     XMLDResponse_add_row(work->resp);
     while (XMLDList_next(work->req->retr)) {
      XMLDResponse_add_col(work->resp);
      if (((XMLDExpr *) XMLDList_curr(work->req->retr))->aggr == 1) {
       XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(work->req->retr), XMLDResponse_curr_col(work->resp));
      }
      else {
       XMLDResponse_fill_col(work->resp, (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr)));
      }
     }
    }
    ret=(*(work->res->engine->walk)) (work);
   }
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_curr_aggr_expr(work->resp) != NULL) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, XMLDResponse_curr_aggr_expr(work->resp)));
    XMLDResponse_next_aggr(work->resp);
   }
   char *resp=resptrans_handle(work);
   xmld_socket_write(work->conn->fd, resp);
   free(resp);
   (*(work->res->engine->cleanup)) (work);
  break;
  case XMLD_SQL_SELECT_WHERE:
   work->res=XMLDResource_create();
  
   full_file=XMLDWork_get_full_file(work);
   int priv=authman_get_priv(work->conn->user, full_file);
   
   if (!BIT_ISSET(priv, XMLD_PRIV_READ)) {
    free(full_file);
    xmld_errno=XMLD_ENORPRIV;
    return XMLD_FAILURE;
   }
   
   work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
   free(full_file);
   
   if (work->res->engine == NULL) {
    xmld_errno=XMLD_ENOENGINE;
    return XMLD_FAILURE;
   }
   
   if (((*(work->res->engine->prepare)) (work, XMLD_ACCESS_FORMAT)) == XMLD_FAILURE) {
    return XMLD_FAILURE;
   }
   
   work->resp=XMLDResponse_create();
   XMLDList_reset(work->req->where);
   ret=(*(work->res->engine->walk)) (work);
   int last_level, curr_level;
   
   while (ret != XMLD_WALK_END) {
    if (ret == XMLD_WALK_DOWN) {
     curr_level = XMLDList_next(work->req->where); /* curr_level is a misleading name, it's to reuse 
						    the same variable */
    }
    else {
     XMLDList_prev(work->req->where);
    }
    if (ret == XMLD_WALK_DOWN) {
     if (curr_level) {
      if ((*(work->res->engine->eval_cond)) (work, (XMLDCond *) XMLDList_curr(work->req->where))) {
       XMLDList_reset(work->req->retr);
       XMLDResponse_add_row(work->resp);
       while (XMLDList_next(work->req->retr)) {
        XMLDResponse_add_col(work->resp);
        if (((XMLDExpr *) XMLDList_curr(work->req->retr))->aggr == 1) {
         XMLDResponse_assoc_col_to_aggr(work->resp, (XMLDExpr *) XMLDList_curr(work->req->retr), XMLDResponse_curr_col(work->resp));
        }
        else {
         XMLDResponse_fill_col(work->resp,  (*(work->res->engine->eval_expr)) (work, (XMLDExpr *) XMLDList_curr(work->req->retr)));
        }
       }
      }
      else { /* the condition isn't true, loop until you reach level-1 */
       last_level = (*(work->res->engine->get_level)) (work);
       ret = (*(work->res->engine->walk)) (work);
       while (ret != XMLD_WALK_END) {
        if (ret == XMLD_WALK_DOWN) {
         XMLDList_next(work->req->where);
        }
        else {
         XMLDList_prev(work->req->where);
        }
        curr_level = (*(work->res->engine->get_level)) (work);
        if (curr_level == last_level-1) {
         break;
        }
        ret = (*(work->res->engine->walk)) (work);
       }
      }
     }
     else { /* levels greater than the number of levels in the condition list
 	    must be eaten up as well: */
      last_level = (*(work->res->engine->get_level)) (work);
      ret = (*(work->res->engine->walk)) (work);
      while (ret != XMLD_WALK_END) {
       if (ret == XMLD_WALK_DOWN) {
        XMLDList_next(work->req->where);
       }
       else {
        XMLDList_prev(work->req->where);
       }
       curr_level = (*(work->res->engine->get_level)) (work);
       if (curr_level == last_level-1) {
        break;
       }
       ret = (*(work->res->engine->walk)) (work);
      }
     }
    } 
    ret=(*(work->res->engine->walk)) (work);   
   }
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_curr_aggr_expr(work->resp) != NULL) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, XMLDResponse_curr_aggr_expr(work->resp)));
    XMLDResponse_next_aggr(work->resp);
   }
   resp=resptrans_handle(work);
   xmld_socket_write(work->conn->fd, resp);
   free(resp);
   (*(work->res->engine->cleanup)) (work);
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
