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
#include "xmld_func.h"
#include "xmld_expr.h"
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
 /* this if() may need to contain other values for type such as
  * SELECT + SORT */
 if (work->req->type == 0) { /* a SELECT query */
  work->res=XMLDResource_create();
  
  char *full_file=XMLDWork_get_full_file(work);
  work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
  free(full_file);
  
  if (work->res->engine == NULL) {
   xmld_errno=XMLD_ENOENGINE;
   return -1;
  }
  
  if (((*(work->res->engine->prepare)) (work)) == 0) {
   return -1;
  }
  
  XMLDExpr *curr_retr;
  work->resp=XMLDResponse_create();
  curr_retr=(XMLDExpr *) XMLDList_first(work->req->retr);
  while ((*(work->res->engine->walk)) (work) != 0) {
   XMLDResponse_add_row(work->resp);
   while (curr_retr != 0) {
    if (curr_retr->aggr == 1) { /* an aggregate expression */
     XMLDResponse_add_col(work->resp);
     XMLDResponse_assoc_col_to_aggr(work->resp, curr_retr, XMLDResponse_curr_col(work->resp));
    }
    else {
     XMLDResponse_add_col(work->resp);
     XMLDResponse_fill_col(work->resp,  (*(work->res->engine->eval_expr)) (work, curr_retr));
    }
   }
   
  }
  XMLDResponse_reset_aggr(work->resp);
  while (XMLDResponse_curr_aggr_expr(work->resp) != NULL) {
   XMLDResponse_fill_curr_aggr(work->resp, (*(work->res->engine->eval_aggr_expr)) (work, XMLDResponse_curr_aggr_expr(work->resp)));
   XMLDResponse_next_aggr(work->resp);
  }
  XMLDResponse_flush(work->resp, work->conn->fd);
  XMLDList_free(work->resp->tables);
  (*(work->res->engine->cleanup)) (work);
 }
 if (work->req->type==1) { /* a SELECT + WHERE query */
  /* to be continued */
 }
 return 0;
};
