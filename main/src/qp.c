#include <iostream.h>
#include "errors.h"
#include "xmld_mempool.h"
#include "xmld_types.h"
#include "qp.h"

void qp_handle(void *conn) {
 struct XMLDWork *work=(struct XMLDWork *) malloc(sizeof(struct XMLDWork));
 /* FIXME the number of segments must come from config */
 struct XMLDMemPool *expr_pool=XMLDMemPool_create(sizeof(struct expr), 20, expr_free);
 struct XMLDMemPool *cond_pool=XMLDMemPool_create(sizeof(struct cond), 10, cond_free);
 work->conn=(struct XMLDConnection *) conn;
 work->req=(struct XMLDRequest *) malloc(sizeof(struct XMLDRequest));
 yyin=fopen(work->conn->fd, "r");
 int status=yyparse();
 if (status==-1) {
  XMLDMemPool_unget_all(expr_pool);
  XMLDMemPool_unget_all(cond_pool);
  return;
 }
 /* process req */ 
}

void expr_free(void *segment) {
}
void cond_free(void *segment) {
}
