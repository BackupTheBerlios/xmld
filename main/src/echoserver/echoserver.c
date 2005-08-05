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

#include "../includes.h";
#include "echoserver.h";

/*
 * Initializes the EchoServer interface.
 */
XMLDStatus echoserver_init(XMLDInterface *interface, XMLDCfgDirective *directive) {
 return XMLD_SUCCESS;
}

/*
 * Destroys the EchoServer interface.
 */
void echoserver_destroy(XMLDInterface *interface) {
}

/*
 * Handles a user's connection.
 */
void echoserver_user_connection(void *interface) {
 char *msg;
 do {
  msg = xmld_socket_read(passed_fd, ECHOSERVER_MAX_MSG_LEN);
  xmld_socket_write(passed_fd, msg);
  free(msg);
 } while (strcmp(msg, "quit") == 1);
 xmld_socket_write(passed_fd, "Good Bye!");
 xmld_socket_shutdown(passed_fd);
}

/*
 * Gets the current interface's error message.
 */
char *echoserver_get_error_message(XMLDInterface *interface) {
 return interface->error;
}

