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
#include "xmld_list.h"
#include "xmld_connection.h"
#include "cfg.h"
#include "mtasker.h"
#include "somanager.h"
#include "sosel.h"

int *fds;
int *ports;
int num_sock;

short somanager_init() {
 num_sock=*((int *) cfg_get("somanager.num_listeners"));
 ports=(int *) cfg_get("somanager.listeners");
 fds=(int*) malloc(num_sock*sizeof(int));
 int i;
 int status;

 for (i=0;i<num_sock;i++) {
  fds[i]=xmld_socket_create();
  
  if (fds[i]==-1) {
   perror("xmld_socket_create");
   return -1;
  }
  status=xmld_socket_bind(fds[i], ports[i]);
  if (status==-1) {
   perror("xmld_socket_bind");
   return -1;
  }
  status=xmld_socket_listen(fds[i]);
  if (status==-1) {
   perror("xmld_socket_listen");
   return -1;
  }
  status=mtasker_handle(somanager_handle, (void*) fds[i]);
  if (status!=0) {
   perror("mtasker_handle");
   return -1;
  }
 }
 return 0;
}

short somanager_shutdown() {
 int i;
 int status;
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
 return 0;
}

void somanager_handle(void *sockfd) {
 int status;
 while (1) {
  status=xmld_socket_accept((int)sockfd);
  if (status==-1) {
   perror("xmld_socket_accept");
   return;
  }
  sosel_add(status, "/");
 } 
}
