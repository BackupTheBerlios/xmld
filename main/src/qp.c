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
 
#include "includes.h"
#include "xmld_sockets.h"
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
 char *arg_carry[8]={COL_SEP_FIELD, COL_SEP_ENC_FIELD, ROW_SEP_FIELD, ROW_SEP_ENC_FIELD, DOWN_LEVEL_FIELD
                     , DOWN_LEVEL_ENC_FIELD, UP_LEVEL_FIELD, UP_LEVEL_ENC_FIELD};
 char *val_carry[8];
 
 /* NUL attachment to the end of single characters */
 char col_sep_str[2];
 char row_sep_str[2];
 char down_level_str[2];
 char up_level_str[2];
 col_sep_str[0]=col_sep;
 row_sep_str[0]=row_sep;
 down_level_str[0]=down_level;
 up_level_str[0]=up_level;
 col_sep_str[1]=row_sep_str[1]=down_level_str[1]=up_level_str[1]='\0';
 
 val_carry[0]=col_sep_str;
 val_carry[1]=col_sep_enc;
 val_carry[2]=row_sep_str;
 val_carry[3]=row_sep_enc;
 val_carry[4]=down_level_str;
 val_carry[5]=down_level_enc;
 val_carry[6]=up_level_str;
 val_carry[7]=up_level_enc;
 char *init_msg=protoimpl_compose_msg(arg_carry, val_carry, 8, 0);
 
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
  
  if (strcmp(query, DISCONNECTION_MESSAGE) == 0) {
   break;
  }

  printf("%s\n", query);
  YY_BUFFER_STATE buf=yy_scan_string(query);
  XMLDStatus status=yyparse((void *) work);
  yy_delete_buffer(buf);
  if (status == 1) {
   ERROR_RESPONSE;
   XMLDResponse_free(work->resp);
   XMLDRequest_free(work->req);
   XMLDList_free(work->files);
   work->req=NULL;
   work->resp=NULL;
   work->files=NULL;
   continue;
  }
  
  free(query);
  status = twalker_handle(work);
  if (status == XMLD_FAILURE) {
   ERROR_RESPONSE;
  }

  XMLDResponse_free(work->resp);
  XMLDRequest_free(work->req);
  XMLDList_free(work->files);
  work->req=NULL;
  work->resp=NULL;
  work->files=NULL;
 }
 xmld_socket_shutdown(work->conn->fd);
 XMLDWork_free(work);
}
