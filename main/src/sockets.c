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
 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "sockets.h"
#include <errno.h>

int socket_create() {
 return socket(PF_INET, SOCK_STREAM, 0);
}
int socket_bind(int sockfd, int port) {
 struct sockaddr_in addr;
 addr.sin_family=AF_INET;
 addr.sin_port=htons(port);
 addr.sin_addr.s_addr=htonl(INADDR_ANY);
 return bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)); 
}
int socket_listen(int sockfd) {
 return listen(sockfd, 5);
}
int socket_shutdown(int sockfd) {
 return shutdown(sockfd, 0);
}
int socket_accept(int sockfd) {
 return accept(sockfd, NULL, NULL);
}

/*
 * Writes a message with maximum length 'len'
 * to sockfd.
 */ 
int socket_write(int sockfd, char *str) {
 return send(sockfd, (void *) str, (strlen(str)+1) * sizeof(char), 0);
}

/*
 * Reads a message with maximum length 'len' from
 * sockfd.
 */ 
char *socket_read(int sockfd, int len) {
 char *ret=(char *) malloc((len + 1) * sizeof(char));
 errno = 0;
 recv(sockfd, (void *) ret, len * sizeof(char), 0);
 ret[len] = '\0';
 return ret; 
}
