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
#include "func_list.h"
#define NUM_PARTS 5
 
struct xmld_part parts[NUM_PARTS];

int main() {
 int t;
 int s;
 
 struct sigaction action;
 action.sa_handler=init_shutdown_parts;
 s=sigaction(SIGINT, &action, NULL);
 
 if (s == -1) {
  perror("sigaction");
  return 1;
 }
 
 printf("The OpenXMLD is up and running:\n\t* Main PID: %d\n", getpid());
 
 /* Error Messages */
 err_str[0]="The request file couldn't be opened.";
 err_str[1]="The given engine does not exist.";
 err_str[2]="Invalid file type.";
 err_str[3]="Invalid use of an aggregate expression.";
 err_str[4]="An error has occured while parsing the given query.";
 err_str[5]="The given query is not yet implemented.";
 err_str[6]="Couldn't open a description file for the request file.";
 err_str[7]="Mal-formed description file for the request file.";
 err_str[8]="Read privileges not available for the request file";
 err_str[9]="Write privileges not available for the request file";

 init_create_part(&parts[0], cfg_init, cfg_shutdown);
 init_create_part(&parts[1], engine_list_init, engine_list_shutdown);
 init_create_part(&parts[2], func_list_init, func_list_shutdown);
 init_create_part(&parts[3], mtasker_init, mtasker_shutdown);
 init_create_part(&parts[4], somanager_init, somanager_shutdown);
 
 for (t = 0; t < NUM_PARTS; t++) {
  s = (*(parts[t].init_func))();
  if (s == XMLD_FAILURE) {
   perror("Initializer");
   init_shutdown_parts(0);
   break;
  }
  else {
   parts[t].ok=XMLD_TRUE;
  }
 }
 while (1) {
 }
 return 0;
}

void init_create_part(struct xmld_part *part, XMLDStatus (*init_func) (void), XMLDStatus (*shutdown_func) (void)) {
 part->init_func=init_func;
 part->shutdown_func=shutdown_func;
 part->ok=XMLD_FAILURE;
}

void init_shutdown_parts(int signum) {
 mtasker_shutdown();
 somanager_shutdown();
 func_list_shutdown();
 engine_list_shutdown();
 cfg_shutdown();
 exit(0);
}
