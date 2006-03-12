
/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#include "error.h"

char *Error_get_message(Error *error) {
 return error->message;
}

void Error_set(Error *error, char *message, ErrorLevel level) {
 error->message = message;
 error->level = level;
}

ErrorLevel Error_get_level(Error *error) {
 return error->level;
}
