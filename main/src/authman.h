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
#define XMLD_PRIV_READ 0  /* Read  */
#define XMLD_PRIV_WRITE 1 /* Write */

#define AUTH_USER_NAME "name"
#define AUTH_USER_PASS "pass"
#define AUTH_USER_DIR "dir"
#define AUTH_FILE "auth.xml"
#define AUTH_TAG "<user"
#define AUTH_USER_HEADER_FIELD "user-name"
#define AUTH_PASS_HEADER_FIELD "pass"

XMLDStatus authman_handle(int , char **);
XMLDStatus authman_auth_user(char *, char **ret);
int authman_get_priv(char *, char *); /* priv = privileges */

#endif /* __AUTHMAN_H */
