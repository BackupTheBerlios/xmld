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

#include "errors.h"
#include "xmld_types.h"

xmld_status_t twalker_handle(struct XMLDWork *work) {
 int status;
 struct expr *curr_retr;
 struct cond *curr_cond;
 XMLDResponse_init(work->resp);
 /* this if() may need to contain other values for type such as
  * SELECT + SORT */
 if (work->req->type==0) { /* a SELECT query */
  curr_retr=work->req->retr[0];
  while ((status=(*(work->res->engine->walk)) (work))!=-1) {
   XMLDResponse_add_row(work->resp);
   while (curr_retr!=0) {
    if (curr_retr->aggr==1) { /* an aggregate expression */
     XMLDResponse_add_aggr(work->resp, curr_retr);
    }
    else {
     XMLDResponse_add_col(work->resp, (*(work->res->engine->eval_expr)) (work, curr_retr));
    }
   }
  }
 }
 if (work->req->type==1) { /* a SELECT + WHERE query */
  /* to be continued */
 }
 XMLDResponse_aggr_reset(work->resp);
 while (XMLDResponse_curr_aggr(work->resp)!=0) {
  XMLDResponse_fill_aggr(work->resp, (*(work->res->engine->eval_aggr_expr)) (work, XMLDResponse_curr_aggr(work->resp)));
  XMLDResponse_aggr_next(work->resp);
 }
 return XMLD_SUCCESS;
};
