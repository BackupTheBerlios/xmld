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

#ifndef __XMLD_WORK_H
#define __XMLD_WORK_H

struct XMLDWork {
 XMLDRequest *req;
 XMLDResponse *resp;
 XMLDConnection *conn;
 XMLDResource *res;
};

typedef struct XMLDWork XMLDWork;
typedef XMLDList XMLDWorkList;

XMLDWork *XMLDWork_create(void);
char *XMLDWork_get_full_file(XMLDWork *);
void XMLDWork_free(XMLDWork *);
void XMLDWork_free_content(void *);

/* List functions */

XMLDList *XMLDWork_create_list(void);
XMLDWork *XMLDWork_add_to_list(XMLDList *);

#endif /* __XMLD_WORK_H */
