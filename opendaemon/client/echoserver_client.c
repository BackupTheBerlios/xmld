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
#include "../sockets.h"

int main() {
 int sockfd = socket(PF_INET, SOCK_STREAM, 0); 
 struct sockaddr_in serv_addr;
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = 0;
 serv_addr.sin_port = htons(8080);
 int s = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
 socket_write(sockfd, "Hello World!");
 char *ret = socket_read(sockfd, 12);
 printf("%s\n", ret);
 return 0;
}
