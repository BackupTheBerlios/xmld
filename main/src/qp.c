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
#include <stdio.h>
#include <string.h>
#include "xmld_sockets.h"
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
#include "sosel.h"
#include "qp.h"
#include "twalker.h"
struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *);

void qp_handle(void *conn) {
 XMLDWork *work=XMLDWork_create();
 work->conn=XMLDConnection_create(((XMLDConnection *) conn)->fd, ((XMLDConnection *) conn)->curr_dir);
 sosel_remove(conn);
 work->req=XMLDRequest_create();
 
 char *query=xmld_socket_read(work->conn->fd);
 yy_scan_string(query);
 int status=yyparse((void *) work);
 if (status == 1) {
  ERROR_RESPONSE;
  sosel_add(work->conn->fd, work->conn->curr_dir);
  XMLDWork_free(work);
  return;
 }
 
 status=twalker_handle(work);
 if (status==-1) {
  ERROR_RESPONSE;
  sosel_add(work->conn->fd, work->conn->curr_dir);
  XMLDWork_free(work);
  return;
 }

 sosel_add(work->conn->fd, work->conn->curr_dir);
 XMLDWork_free(work);
}
