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
  s = mtasker_handle(somanager_handle, (void *) (fds+t), fds[t]);
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

void somanager_handle(void *sockfd) {
 int s;
#ifdef MULTI_PROC_MTASKER
 s = xmld_socket_listen(passed_fd);
#else
 s = xmld_socket_listen(*((int *) sockfd));
#endif /* MULTI_PROC_MTASKER */
 
 if (s == -1) {
  perror("xmld_socket_listen");
  return;
 }

 while (1) {
#ifdef MULTI_PROC_MTASKER
  s = xmld_socket_accept(passed_fd);
#else
  s = xmld_socket_accept(*((int *) sockfd));
#endif /* MULTI_PROC_MTASKER */

  if (s == -1) {
   perror("xmld_socket_accept");
   continue;
  }
  
  XMLDConnection conn;
  conn.fd=s;
  mtasker_handle(qp_handle, (void *) &conn, s);
 } 
}
