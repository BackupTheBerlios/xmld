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

#ifndef __AUTHMAN_H
#define __AUTHMAN_H

/* Bitmask returned by authman_get_priv -- for now! */
#define XMLD_PRIV_NOTHING 0 /* Nothing! */
#define XMLD_PRIV_READ 1  /* Read  */
#define XMLD_PRIV_WRITE 2 /* Write */

#define AUTH_USER_PASS "pass"
#define AUTH_USER_DIR "dir"
#define AUTH_TOK_LEVEL 1
#define AUTH_PRIV_LEVEL 2
#define AUTH_PATH "path"
#define AUTH_PRIV "priv"

XMLDStatus authman_handle(int , char **);
XMLDStatus authman_auth_user(char *, char **ret);
int authman_get_priv(char *, char *); /* priv = privileges */

#endif /* __AUTHMAN_H */
