#ifndef HAVE_XMLD_SOCKETS_H
#define HAVE_XMLD_SOCKETS_H

int xmld_socket_create(void);
int xmld_socket_bind(int, int);
int xmld_socket_listen(int);
int xmld_socket_shutdown(int);
int xmld_socket_accept(int);

#endif /* HAVE_XMLD_SOCKETS_H */
