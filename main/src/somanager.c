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
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "xmlddef.h"
#include "xmld_sockets.h"
#include "xmld_list.h"
#include "xmld_connection.h"
#include "cfg.h"

#ifdef USE_PTASKER
 #include <sys/types.h>
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER 
#endif /* USE_PTASKER */

#include "somanager.h"
#include "qp.h"

int *fds;
int *ports;
int num_sock;

XMLDStatus somanager_init() {
 num_sock=*((int *) cfg_get("somanager.num_listeners"));
 ports=(int *) cfg_get("somanager.listeners");
 fds=(int*) malloc(num_sock*sizeof(int));
 int t;
 int s;

 for (t = 0; t < num_sock; t++) {
  fds[t]=xmld_socket_create();
  
  if (fds[t] == -1) {
   perror("xmld_socket_create");
   return XMLD_FAILURE;
  }
  s = xmld_socket_bind(fds[t], ports[t]);
  if (s == -1) {
   perror("xmld_socket_bind");
   return XMLD_FAILURE;
  }
  printf("\t* Listening port %d: %d\n", t+1, ports[t]);
  s = mtasker_handle(somanager_handle, (void *) &t, fds[t]);
  if (s == XMLD_FAILURE) {
   perror("mtasker_handle");
   return XMLD_FAILURE;
  }
 }
 return XMLD_SUCCESS;
}

XMLDStatus somanager_shutdown() {
 int i;
 for (i = 0; i < num_sock; i++) {
  xmld_socket_shutdown(fds[i]);
 }
 free(fds);
 free(ports);
 return XMLD_SUCCESS;
}

void somanager_handle(void *sockindex) {
 int s;
#ifdef MULTI_PROC_MTASKER
 s = xmld_socket_listen(passed_fd);
#else
 s = xmld_socket_listen(fds[*((int *) sockindex)]);
#endif /* MULTI_PROC_MTASKER */
 
 if (s == -1) {
  perror("xmld_socket_listen");
  return;
 }

 while (1) {
#ifdef MULTI_PROC_MTASKER
  s = xmld_socket_accept(passed_fd);
#else
  s = xmld_socket_accept(fds[*((int *) sockfd)]);
#endif /* MULTI_PROC_MTASKER */

  if (s == -1) {
   perror("xmld_socket_accept");
   continue;
  }
  
  XMLDConnection conn;
  conn.fd = s;
  conn.user = cfg_get_interface_name(ports[*((int *) sockfd)]);
  mtasker_handle(somanager_user_connection, (void *) &conn, s);
 } 
}

/*
 * Handles a new user connection - along with a loop
 * for handling an infinite number of requests.
 */
void somanager_user_connection(void *conn) {
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
  free(conn->user);
  return;
 }
 free(init_msg);
 
 if (authman_handle(fd, val_carry) == XMLD_FAILURE) {
  xmld_socket_shutdown(fd);
  free(conn->user);
  return;
 }
 
 XMLDWork *work=XMLDWork_create();
 work->interface = XMLDInterfaceList_search_by_name(interface_list, conn->user);
 free(conn->user);
 
 if (work->interface == NULL) {
  xmld_socket_shutdown(fd);
  return;
 }

 if (*(work->interface->prepare_conn) (work) == XMLD_FAILURE) {
  xmld_socket_shutdown(fd);
  return;
 }
 
 work->conn=XMLDConnection_create(fd, val_carry[1], val_carry[0]);
 
 while (1) {
  if (*(work->interface->prepare) (work) == XMLD_FAILURE) {
   break;
  }
  char *query=protoimpl_read_sequence(work->conn->fd, NULL);
  
  if (strcmp(query, DISCONNECTION_MESSAGE) == 0) {
   break;
  }

  if (*(work->interface->parse) (work, query) == XMLD_FAILURE) {
   free(query);
   ERROR_RESPONSE;
   *(work->interface->cleanup) (work);
   continue;
  }
 
  free(query);
  if (*(work->interface->walk) (work) == XMLD_FAILURE) {
   ERROR_RESPONSE;
   *(work->interface->cleanup) (work);
   continue;
  }
  
  query = *(work->interface->get_response) (work); /* The response and NOT the query */
  protoimpl_write_sequence(work->conn->fd, query, 1);
  free(query);
  *(work->interface->cleanup) (work);
 }
 *(work->interface->cleanup_conn) (work);
 xmld_socket_shutdown(work->conn->fd);
 XMLDWork_free(work);
}
