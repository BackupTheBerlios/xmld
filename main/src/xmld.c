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
#include "xmld_part.h"
#include "init.h"

XMLDPart parts[4];
xmld_status_t status;
int i;

int main() {
 XMLDPart_create(&parts[0], cfg_init, cfg_shutdown);
 XMLDPart_create(&parts[1], mtasker_init, mtasker_shutdown);
 XMLDPart_create(&parts[2], somanager_init, somanager_shutdown);
 XMLDPart_create(&parts[3], sosel_init, sosel_shutdown);
 
 for (i=0;i<4;i++) {
  status=XMLDPart_start(&parts[i]);;
  if (status!=XMLD_SUCCESS) {
   perror("Initializer");
   init_shutdown_parts();
  }
 }
 return XMLD_SUCCESS;
}

void init_shutdown_parts() {
 for (i=3;i>=0;i--) {
  XMLDPart_shutdown(&parts[i]);
 }
}
