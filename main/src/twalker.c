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
#include "xmld_errors.h"
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

short twalker_handle(XMLDWork *work) {
 switch(work->req->type) {
  case 0: /* SELECT */
   work->res=XMLDResource_create();
  
   char *full_file=XMLDWork_get_full_file(work);
   work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
   free(full_file);
   
   if (work->res->engine == NULL) {
    xmld_errno=XMLD_ENOENGINE;
    return 0;
   }
   
   if (((*(work->res->engine->prepare)) (work)) == 0) {
    return 0;
   }

   work->resp=XMLDResponse_create();
   while (((*(work->res->engine->walk)) (work)) != 0) {
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
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_curr_aggr_expr(work->resp) != NULL) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, XMLDResponse_curr_aggr_expr(work->resp)));
    XMLDResponse_next_aggr(work->resp);
   }
   XMLDResponse_flush(work->resp, work->conn->fd);
   XMLDList_free(work->resp->tables);
   (*(work->res->engine->cleanup)) (work);
  break;
  case 1: /* SELECT with WHERE */
   work->res=XMLDResource_create();
  
   full_file=XMLDWork_get_full_file(work);
   work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
   free(full_file);
   
   if (work->res->engine == NULL) {
    xmld_errno=XMLD_ENOENGINE;
    return 0;
   }
   
   if (((*(work->res->engine->prepare)) (work)) == 0) {
    return 0;
   }
   
   work->resp=XMLDResponse_create();
   int level=(*(work->res->engine->walk)) (work);
   int curr_level;
   while (level != 0) {
    if ((*(work->res->engine->eval_cond)) (work, (XMLDCond *) ((work->req->where->content)+level-1))) {
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
    else { /* the condition isn't true, loop until you reach level - 1 */
     curr_level=level;
     while (level > curr_level - 1) {
      level=(*(work->res->engine->walk)) (work);
     }
    }
    level=(*(work->res->engine->walk)) (work);   
   }
   XMLDResponse_reset_aggr(work->resp);
   while (XMLDResponse_curr_aggr_expr(work->resp) != NULL) {
    XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_expr)) (work, XMLDResponse_curr_aggr_expr(work->resp)));
    XMLDResponse_next_aggr(work->resp);
   }
   XMLDResponse_flush(work->resp, work->conn->fd);
   XMLDList_free(work->resp->tables);
   (*(work->res->engine->cleanup)) (work);
  break;
  case 2: /* UPDATE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 3: /* UPDATE with WHERE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 4: /* DELETE or (DELETE *) */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 5: /* DELETE with WHERE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 6: /* INSERT with column list */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 7: /* INSERT with column list with WHERE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 8: /* INSERT with WHERE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 9: /* INSERT */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
  case 10: /* USE */
   xmld_errno=XMLD_ENOTIMPL;
   return 0;
  break;
 } 
 return 1;
};
