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
#include "xmld_list.h"
#include "xmld_connection.h"

/*
 * : Creates a new connection structure.
 * fd (optional): the client socket of the connection.
 * curr_dir (optional): the current directory of the connection.
 * Note: dir is copied, and not referenced.
 * returns: the newly created connection structure.
 */
XMLDConnection *XMLDConnection_create(int fd, char *curr_dir) {
 XMLDConnection *conn=(XMLDConnection *) malloc(sizeof(XMLDConnection));
 conn->fd=fd;
 if (curr_dir != NULL) {
  conn->curr_dir=(char *) malloc(strlen(curr_dir)*sizeof(char));
  strcpy(conn->curr_dir, curr_dir);
 } 
 return conn;
}

/*
 * : Frees an allocated connection structure.
 * conn: the connection structure to free.
 */
void XMLDConnection_free(XMLDConnection *conn) {
 XMLDConnection_free_content((void *) conn);
 free(conn);
}

/*
 * : Frees the memory internally allocated by a connection structure.
 * conn: the connection structure to free.
 */
void XMLDConnection_free_content(void *conn) {
 free(((XMLDConnection *) conn)->curr_dir); 
}

/*
 * : Creates a list of connection structures.
 * returns: the newly created list.
 */
XMLDList *XMLDConnection_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDConnection), XMLDConnection_free_content);
 return list;
}

/*
 * : Adds a new element to a list of connection structures.
 * fd, dir: see XMLDConnection_create
 * returns: a pointer to the newly added element.
 */
XMLDConnection *XMLDConnection_add_to_list(XMLDList *list, int fd, char *curr_dir) {
 XMLDConnection *conn=(XMLDConnection *) XMLDList_add(list);
 conn->fd=fd;
 if (curr_dir != NULL) {
  conn->curr_dir=(char *) malloc(strlen(curr_dir)*sizeof(char));
  strcpy(conn->curr_dir, curr_dir);
 } 
 return conn;
}
