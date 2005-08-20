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
Status echoserver_init(Interface *interface, CfgDirective *directive) {
 return SUCCESS;
}

/*
 * Destroys the EchoServer interface.
 */
void echoserver_destroy(Interface *interface) {
}

/*
 * Handles a user's connection.
 */
void echoserver_user_connection(void *interface) {
 char *msg;
 do {
  msg = socket_read(passed_fd, ECHOSERVER_MAX_MSG_LEN);
  socket_write(passed_fd, msg);
  free(msg);
 } while (strcmp(msg, "quit") == 1);
 socket_write(passed_fd, "Good Bye!");
 socket_shutdown(passed_fd);
}

/*
 * Gets the current interface's error message.
 */
char *echoserver_get_error_message(Interface *interface) {
 return interface->error;
}

