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
#include <string.h>
#include "mfigure.h"

/*
 * FIXME: use a MIME detection technique, either depending
 * on extension or on reading a bit of the given file,
 * for now, text/xml is always returned.
 */ 
char *mfigure_get_mime(char *file) {
 char *mime=(char *) malloc(9*sizeof(char));
 strcpy(mime, "text/xml");
 return mime;
}
