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
 
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "xmld_sockets.h"
#include "xmld_types.h"
#include "errors.h"
#include "cfg.h"
#include "mtasker.h"
#include "somanager.h"
#include "sosel.h"

int *fds;
int *ports;
int num_sock;
int status;
xmld_status_t stat;
int i;
int j;

/*
 * FIXME: find a safe way of getting ports from cfg
 */ 
xmld_status_t somanager_init() {
 num_sock=cfg_get("somanager.num_listeners");
 fds=(int*) malloc(num_sock*sizeof(int));
 ports=(int*) malloc(num_sock*sizeof(int));
 int port=6060;
 
 for (i=0;i<num_sock;i++) {
  fds[i]=xmld_socket_create();
  ports[i]=port+i;
  
  if (fds[i]==-1) {
   perror("xmld_socket_create");
   return XMLD_FAILURE;
  }
  status=xmld_socket_bind(fds[i], ports[i]);
  if (status==-1) {
   perror("xmld_socket_bind");
   return XMLD_FAILURE;
  }
  status=xmld_socket_listen(fds[i]);
  if (status==-1) {
   perror("xmld_socket_listen");
   return XMLD_FAILURE;
  }
  stat=mtasker_handle(somanager_handle, (void*) fds[i]);
  if (stat!=XMLD_SUCCESS) {
   perror("mtasker_handle");
   return XMLD_FAILURE;
  }
 }
 return XMLD_SUCCESS;
}

xmld_status_t somanager_shutdown() {
 for (i=0;i<num_sock;i++) {
  if (fds[i]!=-1) {
   status=xmld_socket_shutdown(fds[i]);
   if (status==-1) {
    perror("xmld_socket_shutdown");
   }
  }
 }
 free(fds);
 free(ports);
 return XMLD_SUCCESS;
}

void somanager_handle(void *sockfd) {
 while (1) {
  status=xmld_socket_accept((int)sockfd);
  if (status==-1) {
   perror("xmld_socket_accept");
   return;
  }
  sosel_add(status, "/");  
 } 
}
