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
 
#ifndef HAVE_XMLD_CONNECTION_H
#define HAVE_XMLD_CONNECTION_H

/* carries connection-specific information */
struct XMLDConnection {
 int fd;
 int sfd;
 char *curr_dir; /* the current directory */
};

typedef struct XMLDConnection XMLDConnection;

XMLDConnection *XMLDConnection_create(int, int, char *);
void XMLDConnection_free_content(XMLDConnection *);
void XMLDConnection_free(XMLDConnection *);

#endif /* HAVE_XMLD_CONNECTION_H */