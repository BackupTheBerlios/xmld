#include "errors.h"
#include "xmld_types.h"
#include "resp.h"

xmld_status_t twalker_handle(struct XMLDWork *work) {
 int status;
 struct expr *curr_retr;
 struct cond *curr_cond;
 resp_init(work);
 /* this if() may need to contain other values for type such as
  * SELECT + SORT */
 if (work->req->type==0) { /* a SELECT query */
  curr_retr=work->req->retr[0];
  while ((status=(*(work->res->engine->walk)) (work))!=-1) {
   resp_add_row(work);
   while (curr_retr!=0) {
    if (curr_retr->aggr==1) { /* an aggregate expression */
     resp_add_aggr(work, curr_retr);
    }
    else {
     resp_add_col(work, (*(work->res->engine->eval_expr)) (work, curr_retr));
    }
   }
  }
 }
 if (work->req->type==1) { /* a SELECT + WHERE query */
  /* to be continued */
 }
 resp_aggr_reset(work);
 while (resp_curr_aggr(work)!=0) {
  resp_fill_aggr(work, (*(work->res->engine->eval_aggr_expr)) (work, resp_curr_aggr(work)));
  resp_aggr_next(work);
 }
 return XMLD_SUCCESS;
};
