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

#ifndef __SUTILS_H
#define __SUTILS_H

char **str_split(char *, char);
char *str_prepend(char *, char *);
short str_like(char *, char *);
short str_between(char *, char *, char *);
char *itoa(int);
char *ltrim(char *);
char *mstrchr_replace(char *, char*, char **, int);

#endif /* __SUTILS_H */

