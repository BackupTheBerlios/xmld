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
 
#include <iostream.h>
#include "xmld_mempool.h"
#include "xmld_types.h"
#include "qp.h"

void qp_handle(void *conn) {
 struct XMLDWork *work=(struct XMLDWork *) malloc(sizeof(struct XMLDWork));
 /* FIXME the number of segments must come from config */
 struct XMLDMemPool *expr_pool=XMLDMemPool_create(sizeof(struct expr), 20, expr_free);
 struct XMLDMemPool *cond_pool=XMLDMemPool_create(sizeof(struct cond), 10, NULL);
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