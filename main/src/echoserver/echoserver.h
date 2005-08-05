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

#ifndef __ECHO_SERVER_H
#define __ECHO_SERVER_H

#define ECHOSERVER_MAX_MSG_LEN 100

XMLDStatus echoserver_init(XMLDInterface *, XMLDCfgDirective *);
void echoserver_destroy(XMLDInterface *);
void echoserver_user_connection(void *);
char *echoserver_get_error_message(XMLDInterface *);

#endif /* __ECHO_SERVER_H */
