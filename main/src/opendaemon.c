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
#include "ptasker/ptasker.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "cfg.h"
#include "opendaemon.h"
#include "connman.h"
#include "engine_list.h"
#include "interface_list.h"
 
int main() {
 /* Signal Handlers */
 int s;
 
 struct sigaction action;
 action.sa_handler=shutdown;
 s = sigaction(SIGINT, &action, NULL);
 
 if (s == -1) {
  perror("sigaction");
  return 1;
 }

 struct sigaction cfg_action;
 cfg_action.sa_handler=update_config;
 s = sigaction(SIGUSR2, &cfg_action, NULL);

 if (s == -1) {
  perror("sigaction");
  return 1;
 }
 
 printf("* Initializing the main configuration tree ...\n");

 if (cfg_init() == FAILURE) {
  printf("\t* An error has occured while trying to create the configuration tree.");
  return 1;
 }
 
 printf("* Initializing the module loader ...\n");
 
 if (modman_init() == FAILURE) {
  cfg_shutdown();
  printf("\t* Error initializing the module loader.");
  return 1;
 }
 
 printf("* Initializing the main process pool ...\n");
 
 CfgSection *ptasker_section = CfgSection_get_section(cfg_tree, "MainProcessPool", 0);
 if (ptasker_section == NULL) {
  printf("\t* There's no configuration section for the main process pool.");
  cfg_shutdown();
  return 1;
 }

 CfgDirective *curr_directive = CfgSection_get_directive(ptasker_section, "InitProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the initial number of processes.");
  cfg_shutdown();
  return 1;
 }

 CfgValue *curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the initial number of processes directive.");
  cfg_shutdown();
  return 1;
 }

 int init_proc = (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the maximum number of processes.");
  cfg_shutdown();
  return 1;
 }

 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the maximum number of processes directive.");
  cfg_shutdown();
  return 1;
 }
 
 int max_proc= (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxIdleProc", 0);
 if (curr_directive == NULL) {
  printf("\t* There's no configuration directive for the maximum number of idle processes.");
  cfg_shutdown();
  return 1;
 }
 
 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  printf("\t* Undefined or type mismatch value for the maximum number of idle processes directive.");
  cfg_shutdown();
  return 1;
 }
 int max_idle_proc= (int) curr_value->value;
 
 main_proc_pool = ptasker_create_pool(init_proc, max_proc, max_idle_proc, "opendaemon.c");
 
 if (main_proc_pool != NULL) {
  printf("* Main PID: %d\n", getpid());
 }
 else {
  cfg_shutdown();
  print("\tError initializing the main process pool ...");
  return 1;
 }

 interfaceman_init();

 while (1) {
 }
 
 return 0;
}

void shutdown(int signum) {
 ptasker_destroy_pool(main_proc_pool);
 cfg_shutdown();
 exit(0);
}

void update_config(int signum) {
 cfg_shutdown();
 cfg_init();
 modman_init();
}
