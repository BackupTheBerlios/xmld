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
#include "qp.h"
#include "twalker.h"

#ifdef USE_PTASKER
 #include <sys/types.h>
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER
#endif /* USE_PTASKER */

#include <sys/socket.h> /* remove that when you are done */
struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *);
void yy_switch_to_buffer(YY_BUFFER_STATE);
void yy_delete_buffer(YY_BUFFER_STATE);

void qp_handle(void *conn) {
 while (1) {
  XMLDWork *work=XMLDWork_create();
#ifdef MULTI_PROC_MTASKER
  work->conn=XMLDConnection_create(passed_fd, ((XMLDConnection *) conn)->curr_dir);
#else 
  work->conn=XMLDConnection_create(((XMLDConnection *) conn)->fd, ((XMLDConnection *) conn)->curr_dir);
#endif /* MULTI_PROC_MTASKER */
  work->req=XMLDRequest_create();
  
  char *query=xmld_socket_read(work->conn->fd);
  YY_BUFFER_STATE buf=yy_scan_string(query);
  
  int status=yyparse((void *) work);
  yy_delete_buffer(buf);
  if (status == 1) {
   ERROR_RESPONSE;
   XMLDWork_free(work);
   continue;
  }
  
  free(query);
  status=twalker_handle(work);
  if (status == 0) {
   ERROR_RESPONSE;
   XMLDWork_free(work);
   continue;
  }

  XMLDWork_free(work);
 } 
}
