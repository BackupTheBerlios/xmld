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
 
/* add all the structures manipulation functions here */
#include <stdlib.h>
#include "xmld_types.h"

void expr_free_content(void *segment) {
 struct expr *ex=(struct expr *)segment;
 if (ex->type==2 && ex->ident!=0) {
  free(ex->ident);
 }
 if (ex->type==4 && ex->qval!=0) {
  free(ex->qval);
 }
 if (ex->alias!=0) {
  free(ex->alias);
 }
}
 
