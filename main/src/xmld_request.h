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
 XMLDList *retr;  /* list of action expressions (query type dependent) */
 XMLDList *vals; /* A list of values for the above expressions  (query type dependent) */
 XMLDList *where; /* A condition for each level */
 XMLDList *misc; /* An additional list for other parsers to use */
};

typedef struct XMLDRequest XMLDRequest;

XMLDRequest *XMLDRequest_create(void);
void XMLDRequest_free(XMLDRequest *);
void XMLDRequest_free_content(void *);

/* List functions */

XMLDList *XMLDRequest_create_list(void);
XMLDRequest *XMLDRequest_add_to_list(XMLDList *);

#endif /* __XMLD_REQUEST_H */
