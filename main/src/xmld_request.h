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
 
#ifndef __XMLD_REQUEST_H
#define __XMLD_REQUEST_H

/* carries the parsed query */
struct XMLDRequest {
 char *file;
 int type; /* type of the query */
 XMLDExprList *retr;  /* list of action expressions (query type dependent) */
 XMLDExprList *vals;  /* A list of values for the above expressions  (query type dependent) */
 XMLDExprList *where; /* A condition for each level */
};

typedef struct XMLDRequest XMLDRequest;
typedef XMLDList XMLDRequestList;

XMLDRequest *XMLDRequest_create(void);
void XMLDRequest_free(XMLDRequest *);
void XMLDRequest_free_content(void *);

/* List functions */

XMLDRequestList *XMLDRequestList_create(void);
XMLDRequest *XMLDRequestList_add(XMLDRequestList *);

#endif /* __XMLD_REQUEST_H */
