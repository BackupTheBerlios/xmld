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

#include "connman.h"

Status connman_init(void) {
 int s;
 AssocWalker walker;
 walker.subject = interface_list;
 walker.curr_index = -1;

 while (AssocWalker_next(&walker)) {
  s = mtasker_handle(connman_listener, AssocWalker_get_current_data(&walker), 0);
  if (s == FAILURE) {
   perror("mtasker_handle");
   return FAILURE;
  }
 }
 return SUCCESS;
}

void connman_listener(void *sockindex) {
 int s = socket_create();
 int status;
 
 if (s == -1) {
  perror("socket_create");
  return;
 }
 
 status = socket_bind(s, ((Interface *) sockindex)->port);
 if (status == -1) {
  perror("socket_bind");
  return;
 }
 
 status = socket_listen(s);
 if (status == -1) {
  perror("socket_listen");
  return;
 }

 while (1) {
  status = socket_accept(s);

  if (status == -1) {
   perror("socket_accept");
   continue;
  }
  mtasker_handle(((Interface *) sockindex)->user_connection, sockindex, s);
 } 
}
