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
 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "xmld_sockets.h"

int xmld_socket_create() {
 return socket(PF_INET, SOCK_STREAM, 0);
}
int xmld_socket_bind(int sockfd, int port) {
 struct sockaddr_in addr;
 addr.sin_family=AF_INET;
 addr.sin_port=htons(port);
 addr.sin_addr.s_addr=htonl(INADDR_ANY);
 return bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)); 
}
int xmld_socket_listen(int sockfd) {
 return listen(sockfd, 5);
}
int xmld_socket_shutdown(int sockfd) {
 return shutdown(sockfd, 0);
}
int xmld_socket_accept(int sockfd) {
 return accept(sockfd, NULL, NULL);
}
int xmld_socket_write(int sockfd, char *str) {
 return send(sockfd, (void *) str, strlen(str)*sizeof(char), MSG_NOSIGNAL);
}
