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
 /*
  * Here should go lexing and parsing.
  */
 status=fmanager_handle(work);
 if (status == -1) {
  ERROR_RESPONSE();
  sosel_add(work->conn->fd, work->conn->curr_dir);
  XMLDWork_free(work);
  return;
 }
 status=twalker_handle(work);
 if (status==-1) {
  ERROR_RESPONSE();
  sosel_add(work->conn->fd, work->conn->curr_dir);
  XMLDWork_free(work);
  return;
 }

 XMLDResponse_flush(work->resp, work->conn->fd);
 sosel_add(work->conn->fd, work->conn->curr_dir);
 XMLDWork_free(work);
}
