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
#include "twalker.h"

short twalker_handle(XMLDWork *work) {
 short status;
 /* this if() may need to contain other values for type such as
  * SELECT + SORT */
 if (work->req->type == 0) { /* a SELECT query */
  XMLDExpr *curr_retr;
  XMLDCond *curr_cond;
  work->resp=XMLDResponse_create();
  curr_retr=work->req->retr[0];
  while ((status=(*(work->res->engine->walk)) (work)) != -1) {
   XMLDResponse_add_row(work->resp);
   while (curr_retr != 0) {
    if (curr_retr->aggr == 1) { /* an aggregate expression */
     XMLDResponse_add_aggr(work->resp, curr_retr);
    }
    else {
     XMLDResponse_add_col(work->resp, (*(work->res->engine->eval_expr)) (work, curr_retr));
    }
   }
  }
  XMLDResponse_aggr_reset(work->resp);
  while (XMLDResponse_curr_aggr(work->resp)!=0) {
   XMLDResponse_fill_aggr(work->resp, (*(work->res->engine->eval_aggr_expr)) (work, XMLDResponse_curr_aggr(work->resp)));
   XMLDResponse_aggr_next(work->resp);
  }
  XMLDList_free(work->resp->tables);
 }
 if (work->req->type==1) { /* a SELECT + WHERE query */
  /* to be continued */
 }
 (*(work->res->engine->cleanup)) (work);
 return 0;
};
