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
#include "init.h"
#include "connman.h"
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

 struct sigaction cfg_action;
 cfg_action.sa_handler=update_config;
 s=sigaction(SIGUSR2, &cfg_action, NULL);

 if (s == -1) {
  perror("sigaction");
  return 1;
 }
 
 printf("OpenDaemon is up and running:\n\t* Main PID: %d\n", getpid());

 if (cfg_init() == FAILURE) {
  perror("cfg_init");
  return 1;
 }
 
 if (engine_list_init() == FAILURE) {
  cfg_shutdown();
  perror("engine_list_init");
  return 1;
 }
 
 if (interface_list_init() == FAILURE) {
  cfg_shutdown();
  engine_list_shutdown();
  perror("interface_list_init");
  return 1;
 }
 if (mtasker_init() == FAILURE) {
  cfg_shutdown();
  engine_list_shutdown();
  interface_list_shutdown();
  perror("mtasker_init");
  return 1;
 }
 if (connman_init() == FAILURE) {
  cfg_shutdown();
  engine_list_shutdown();
  interface_list_shutdown();
  mtasker_shutdown();
  perror("connman_init");
  return 1;
 }
 
 while (1) {
 }
 
 return 0;
}

void init_shutdown_parts(int signum) {
 mtasker_shutdown();
 interface_list_shutdown();
 engine_list_shutdown();
 cfg_shutdown();
 exit(0);
}

void update_config(int signum) {
 mtasker_signal_children(SIGUSR2);
 cfg_update(0);
 connman_init();
}
