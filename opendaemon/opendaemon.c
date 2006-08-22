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
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "process_pool.h"
#include "cfg.h"
#include "opendaemon.h"
 
int main() {
 printf("* Creating main configuration tree ...\n");

 if (cfg_init() == FAILURE) {
  printf("\t* An error has occured while trying to create the configuration tree.\n");
  return 1;
 }
 
 printf("* Initializing module loader ...\n");
 
 if (modman_init() == FAILURE) {
  cfg_shutdown();
  printf("\t* Error initializing module loader.\n");
  return 1;
 }
 
 printf("* Initializing main process pool ...\n");
 
 CfgSection *ptasker_section = CfgSection_get_section(cfg_tree, "MainProcessPool", 0);
 if (ptasker_section == NULL) {
  printf("\t* There's no configuration section for main process pool.\n");
  cfg_shutdown();
  return 1;
 }

 CfgDirective *curr_directive = CfgSection_get_directive(ptasker_section, "InitProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the initial number of processes.\n");
  cfg_shutdown();
  return 1;
 }

 CfgValue *curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the initial number of processes directive.\n");
  cfg_shutdown();
  return 1;
 }

 int init_proc = (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the maximum number of processes.\n");
  cfg_shutdown();
  return 1;
 }

 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the maximum number of processes directive.\n");
  cfg_shutdown();
  return 1;
 }
 
 int max_proc = (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxIdleProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the maximum number of idle processes.\n");
  cfg_shutdown();
  return 1;
 }
 
 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the maximum number of idle processes directive.\n");
  cfg_shutdown();
  return 1;
 }
 int max_idle_proc = (int) curr_value->value;
 
 main_proc_pool = process_pool_create(init_proc, max_proc, max_idle_proc);
 
 if (main_proc_pool != NULL) {
  printf("* Main PID: %d\n", getpid());
 }
 else {
  cfg_shutdown();
  printf("\t* Error initializing main process pool ...\n");
  return 1;
 }

 interfaceman_init();

 /* Signal Handlers */
 printf("* Setting up signal handler(s) ...\n");

 int s;
 
 struct sigaction action;
 action.sa_handler=shutdown;
 s = sigaction(SIGINT, &action, NULL);
 
 if (s == -1) {
  perror("sigaction");
  return 1;
 }

 wait(NULL);
 
 return 0;
}

void shutdown(int signum) {
 process_pool_destroy(main_proc_pool);
 cfg_shutdown();
 exit(0);
}
