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
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "sockets.h"

#ifdef USE_PTASKER
 #include <sys/types.h>
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER 
#endif /* USE_PTASKER */

#include "somanager.h"

int *fds;
int *ports;
int num_sock;

Status somanager_init() {
 CfgSection *ports_section = CfgSection_get_section(cfg_tree, "Ports", 0);
 if (ports_section == NULL) {
  return FAILURE;
 }
 CfgDirective *port_directive;
 num_sock = 0;
 while ((port_directive = CfgSection_get_directive(ports_section, "Port", num_sock)) != NULL) {
  CfgValue *port_value = CfgDirective_get_value(port_directive, 0);
  if (port_value->type != CFG_INTEGER) {
   continue;
  }
  ports = (int *) realloc(ports, (num_sock+1)*sizeof(int));
  ports[num_sock] = (int) port_value->value;
  num_sock++;
 }
 
 if (num_sock == 0) {
  return FAILURE;
 }
 fds=(int*) malloc(num_sock*sizeof(int));
 int t;
 int s;

 for (t = 0; t < num_sock; t++) {
  fds[t]=socket_create();
  
  if (fds[t] == -1) {
   perror("socket_create");
   return FAILURE;
  }
  s = socket_bind(fds[t], ports[t]);
  if (s == -1) {
   perror("socket_bind");
   return FAILURE;
  }
  s = mtasker_handle(somanager_handle, (void *) &t, fds[t]);
  if (s == FAILURE) {
   perror("mtasker_handle");
   return FAILURE;
  }
 }
 return SUCCESS;
}

Status somanager_shutdown() {
 int i;
 for (i = 0; i < num_sock; i++) {
  socket_shutdown(fds[i]);
 }
 free(fds);
 return SUCCESS;
}

void somanager_handle(void *sockindex) {
 int s;
#ifdef MULTI_PROC_MTASKER
 s = socket_listen(passed_fd);
#else
 s = socket_listen(fds[*((int *) sockindex)]);
#endif /* MULTI_PROC_MTASKER */
 
 if (s == -1) {
  perror("socket_listen");
  return;
 }

 while (1) {
#ifdef MULTI_PROC_MTASKER
  s = socket_accept(passed_fd);
#else
  s = socket_accept(fds[*((int *) sockindex)]);
#endif /* MULTI_PROC_MTASKER */

  if (s == -1) {
   perror("socket_accept");
   continue;
  }
  Interface *interface = interface_list_search_by_port(ports[*((int *) sockindex)]);
  mtasker_handle(interface->user_connection, (void *) interface, s);
 } 
}
