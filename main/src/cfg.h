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

#ifndef HAVE_CFG_H
#define HAVE_CFG_H

short cfg_init(void);
void *cfg_get(char *);
char *cfg_get_engine(char *);
void cfg_set_engine(char *, char *);
char *cfg_get_mime_engine(char *);
void cfg_set_mime_engine(char *, char *);
short cfg_shutdown(void);

#endif /* HAVE_CFG_H */
