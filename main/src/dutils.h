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

#ifndef __DUTILS_H
#define __DUTILS_H

struct buf_t {
 char *val;
 char *curr;
};

typedef struct buf_t buf_t;

char fgetc_buf(FILE *, buf_t *);
void buf_dump(FILE *, buf_t *);
int dmstrstr(FILE *, char **, int);
int dmstrchr(FILE *, char *, int);
short dmwstrchr(FILE *, char *, int, char *);
char *dmcstrchr(FILE *, char *, int);

#endif /* __DUTILS_H */
