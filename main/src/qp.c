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
#include "sosel.h"
#include "fmanager.h"
#include "qp.h"
#include "twalker.h"

void qp_handle(void *conn) {
 XMLDWork *work=XMLDWork_create();
 work->conn=XMLDConnection_create(((XMLDConnection *) conn)->fd, ((XMLDConnection
 *))->curr_dir);
 sosel_remove(conn);
 work->req=XMLDRequest_create();
 /* FIXME: what should yyin be assigned to ? */
 yyin=fopen(work->conn->fd, "r");
 int status=yyparse();
 if (status==-1) {
  /*
   * should find a safe way to free
   * the memory used inside yyparse
   */
  return;
 }
 status=fmanager_handle(work);
 if (status==-1) {
  /* NOTE: ERROR_RESPONSE must contain readdition of the socket
   * to conn_table (sosel_add) which will be a blocking operation */
  ERROR_RESPONSE("There was an error accessing the requested file.");
  return;
 }
 status=twalker_handle(work);
 if (status==-1) {
  ERROR_RESPONSE("Error during implementing query");
  return;
 }
 /* send through the socket*/
 /* re-add the socket */
}
