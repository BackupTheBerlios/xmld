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

#ifndef HAVE_XMLD_WORK_H
#define HAVE_XMLD_WORK_H

struct XMLDWork {
 XMLDRequest *req;
 XMLDResponse *resp;
 XMLDConnection *conn;
 XMLDResource *res;
};

typedef struct XMLDWork XMLDWork;

XMLDWork *XMLDWork_create(XMLDRequest *, XMLDResponse *, XMLDConnection *, XMLDResource *);
char *XMLDWork_get_full_file(XMLDWork *);
void XMLDWork_free(XMLDWork *);
void XMLDWork_free_content(void *);

/* List functions */

XMLDList *XMLDWork_create_list(void);
XMLDWork *XMLDWork_add_to_list(XMLDList *, XMLDRequest *, XMLDResponse *, XMLDConnection *, XMLDResource *);

#endif
