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
 
#include "cfg.h"
#include "mtasker.h"
#include "somanager.h"
#include "sosel.h"
#include "errors.h"
#include "init.h"

struct xmld_part parts[4];
xmld_status_t status;
int i;

int main() {
 xmld_create_part(&parts[0], cfg_init, cfg_shutdown);
 xmld_create_part(&parts[1], mtasker_init, mtasker_shutdown);
 xmld_create_part(&parts[2], somanager_init, somanager_shutdown);
 xmld_create_part(&parts[3], sosel_init, sosel_shutdown);
 
 for (i=0;i<4;i++) {
  status=(*(parts[i].init_func))();
  if (status!=XMLD_SUCCESS) {
   perror("Initializer");
   xmld_shutdown_parts();
  }
  else {
   parts[i].ok=1;
  }
 }
 return XMLD_SUCCESS;
}

void init_create_part(struct xmld_part *part, xmld_status_t (*init_func) (void), xmld_status_t (*shutdown_func) (void)) {
 part->init_func=init_func;
 part->shutdown_func=shutdown_func;
 part->ok=0;
}
void init_shutdown_parts() {
 for (i=3;i>=0;i--) {
  if (parts[i].ok==1) {
   status=(*(parts[i].shutdown_func))();
   if (status!=XMLD_SUCCESS) {
    printf("init_shutdown_parts");
   }
  } 
 }
}
