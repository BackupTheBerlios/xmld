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
 
#ifndef HAVE_XMLD_SOCKETS_H
#define HAVE_XMLD_SOCKETS_H

int xmld_socket_create(void);
int xmld_socket_bind(int, int);
int xmld_socket_listen(int);
int xmld_socket_shutdown(int);
int xmld_socket_accept(int);
int xmld_socket_write(int, char *);

#endif /* HAVE_XMLD_SOCKETS_H */
