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

#ifndef __ERROR_H
#define __ERROR_H

typedef int ErrorLevel;

/* Error levels */
#define ERROR_WARNING 2
#define ERROR_FATAL 3

struct Error {
 char *message;
 ErrorLevel level;
};

typedef struct Error Error;

char *Error_get_message(Error *);
void Error_set(Error *, char *, ErrorLevel);
ErrorLevel Error_get_level(Error *);

#endif /* __ERROR_H */
