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

#ifdef USE_PTASKER
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER
#endif /* USE_PTASKER */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "cfg.h"
#include "somanager.h"
#include "init.h"
#include "engine_list.h"
#include "interface_list.h"
 
int main() {
 int s;
 
 struct sigaction action;
 action.sa_handler=init_shutdown_parts;
 s=sigaction(SIGINT, &action, NULL);
 
 if (s == -1) {
  perror("sigaction");
  return 1;
 }
 
 printf("OpenDaemon is up and running:\n\t* Main PID: %d\n", getpid());

 cfg_init();
 engine_list_init()
 interface_list_init()
 mtasker_init()
 somanager_init()
 
 while (1) {
 }
 return 0;
}

void init_shutdown_parts(int signum) {
 mtasker_shutdown();
 somanager_shutdown();
 interface_list_shutdown();
 engine_list_shutdown();
 cfg_shutdown();
 exit(0);
}
