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
 
#ifndef __SOCKETS_H
#define __SOCKETS_H

int socket_create(void);
int socket_bind(int, int);
int socket_listen(int);
int socket_shutdown(int);
int socket_accept(int);
int socket_write(int, char *);
char *socket_read(int, int);

#endif /* __SOCKETS_H */
