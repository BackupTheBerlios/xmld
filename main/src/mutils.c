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
#include <stdlib.h>
#include "mutils.h"

/*
 * : Checks for if the given pointer is NULL before
 * calling free on it.
 */
void cfree(void *p) {
 if (p != NULL) {
  free(p);
 }
}
