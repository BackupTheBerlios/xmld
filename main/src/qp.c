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
#include "xmlddef.h"
#include "xmld_sockets.h"
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
#include "cfg.h"
#include "qp.h"
#include "twalker.h"
#include "protoimpl.h"
#include "authman.h"

#ifdef USE_PTASKER
 #include <sys/types.h>
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER
#endif /* USE_PTASKER */

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *);
void yy_switch_to_buffer(YY_BUFFER_STATE);
void yy_delete_buffer(YY_BUFFER_STATE);

void qp_handle(void *conn) {
#ifdef MULTI_PROC_MTASKER
 int fd=passed_fd;
#else
 int fd=((XMLDConnection *) conn)->fd;
#endif
 
 /* writing the init msg */
 char *arg_carry[4]={QP_COL_SEP_FIELD, QP_COL_SEP_ENC_FIELD, QP_ROW_SEP_FIELD, QP_ROW_SEP_ENC_FIELD};
 char *val_carry[4];
 val_carry[0]=&col_sep;
 val_carry[1]=col_sep_enc;
 val_carry[2]=&row_sep;
 val_carry[3]=row_sep_enc;
 char *init_msg=protoimpl_compose_msg(arg_carry, val_carry, 4, 0);
 
 if (protoimpl_write_sequence(fd, init_msg, 1) == XMLD_FAILURE) {
  free(init_msg);
  xmld_socket_shutdown(fd);
  return;
 }
 free(init_msg);
 
 if (authman_handle(fd, val_carry) == XMLD_FAILURE) {
  xmld_socket_shutdown(fd);
  return;
 }
 
 XMLDWork *work=XMLDWork_create();
 work->conn=XMLDConnection_create(fd, val_carry[1], val_carry[0]);
 
 while (1) {
  work->req=XMLDRequest_create();
  
  char *query=protoimpl_read_sequence(work->conn->fd, NULL);
  
  if (strcmp(query, DISCONNECT_MESSAGE) == 0) {
   break;
  }
  
  YY_BUFFER_STATE buf=yy_scan_string(query);
  printf("%s\n", query);
  XMLDStatus status=yyparse((void *) work);
  yy_delete_buffer(buf);
  if (status == 1) {
   ERROR_RESPONSE;
   XMLDWork_free(work);
   continue;
  }
  
  free(query);
  status = twalker_handle(work);
  if (status == XMLD_FAILURE) {
   ERROR_RESPONSE;
   XMLDWork_free(work);
   continue;
  }
  XMLDResponse_free(work->resp);
  XMLDRequest_free(work->req);
  XMLDResource_free(work->res);
 }
 xmld_socket_shutdown(work->conn->fd);
 XMLDWork_free(work);
}
