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
#include "xmlddef.h"
#include "mutils.h"
#include "xmld_list.h"
#include "xmld_connection.h"

/*
 * : Creates a new connection structure.
 * fd (optional): the client socket of the connection.
 * curr_dir (optional): the current directory of the connection.
 * Note: dir is copied, and not referenced.
 * returns: the newly created connection structure.
 */
XMLDConnection *XMLDConnection_create(int fd, char *curr_dir, char *user) {
 XMLDConnection *conn=(XMLDConnection *) malloc(sizeof(XMLDConnection));
 conn->fd=fd;
 conn->curr_dir=curr_dir;
 conn->user=user;
 return conn;
}

/*
 * : Frees an allocated connection structure.
 * conn: the connection structure to free.
 */
void XMLDConnection_free(XMLDConnection *conn) {
 if (conn != NULL) {
  XMLDConnection_free_content((void *) conn);
  free(conn);
 }
}

/*
 * : Frees the memory internally allocated by a connection structure.
 * conn: the connection structure to free.
 */
void XMLDConnection_free_content(void *conn) {
}

/*
 * : Creates a list of connection structures.
 * returns: the newly created list.
 */
XMLDConnectionList *XMLDConnectionList_create() {
 return XMLDList_create(sizeof(XMLDConnection), XMLDConnection_free_content);
}

/*
 * : Adds a new element to a list of connection structures.
 * list: the list to which the new element is to be added.
 * fd, curr_dir: see XMLDConnection_create
 * returns: a pointer to the newly added element.
 */
XMLDConnection *XMLDConnectionList_add(XMLDConnectionList *list, int fd, char *curr_dir, char *user) {
 XMLDConnection *conn=(XMLDConnection *) XMLDList_add(list);
 conn->fd=fd;
 conn->curr_dir=curr_dir;
 conn->user=user;
 return conn;
}
