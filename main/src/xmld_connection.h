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
 
#ifndef __XMLD_CONNECTION_H
#define __XMLD_CONNECTION_H

/* carries connection-specific information */
struct XMLDConnection {
 int fd;
 char *curr_dir; /* the current directory of the connection */
 char *user;
};

typedef struct XMLDConnection XMLDConnection;
typedef XMLDList XMLDConnectionList;

XMLDConnection *XMLDConnection_create(int, char *, char *);
void XMLDConnection_free(XMLDConnection *);
void XMLDConnection_free_content(void *);

/* List functions */

XMLDList *XMLDConnection_create_list(void);
XMLDConnection *XMLDConnection_add_to_list(XMLDList *, int, char *);

#endif /* __XMLD_CONNECTION_H */
