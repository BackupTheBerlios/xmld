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

#include <stdio.h>
#include "cfg.h"

#ifdef USE_PTASKER
 #include <sys/types.h>
 #include "ptasker/ptasker.h"
 #define MULTI_PROC_MTASKER
 #undef MULTI_THREAD_MTASKER
#endif /* USE_PTASKER */

#include "somanager.h"
#include "sosel.h"
#include "init.h"
#include "engine_list.h"
#include "func_list.h"

struct xmld_part parts[6];
short status;
int i;

int main() {
 init_create_part(&parts[0], cfg_init, cfg_shutdown);
 init_create_part(&parts[1], mtasker_init, mtasker_shutdown);
 init_create_part(&parts[2], somanager_init, somanager_shutdown);
 init_create_part(&parts[3], sosel_init, sosel_shutdown);
 init_create_part(&parts[4], engine_list_init, engine_list_shutdown);
 init_create_part(&parts[5], func_list_init, func_list_shutdown);

 for (i=0;i<4;i++) {
  status=(*(parts[i].init_func))();
  if (status!=0) {
   perror("Initializer");
   init_shutdown_parts();
  }
  else {
   parts[i].ok=1;
  }
 }
 return 0;
}

void init_create_part(struct xmld_part *part, short (*init_func) (void), short (*shutdown_func) (void)) {
 part->init_func=init_func;
 part->shutdown_func=shutdown_func;
 part->ok=0;
}
void init_shutdown_parts() {
 for (i=3;i>=0;i--) {
  if (parts[i].ok==1) {
   status=(*(parts[i].shutdown_func))();
   if (status!=0) {
    printf("init_shutdown_parts");
   }
  } 
 }
}
