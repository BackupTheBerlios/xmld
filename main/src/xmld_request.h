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
 
#ifndef HAVE_XMLD_REQUEST_H
#define HAVE_XMLD_REQUEST_H

/* carries the parsed query */
struct XMLDRequest {
 char *file;
 short type; /* type of the query
              * 0 = SELECT query
              * 1 = SELECT with WHERE
              */
 XMLDList *retr;  /* the list of expressions to be
	           * retrieved */
 XMLDList *where; /* A condition for each level */
};

typedef struct XMLDRequest XMLDRequest;

XMLDRequest *XMLDRequest_create(void);
void XMLDRequest_free(XMLDRequest *);
void XMLDRequest_free_content(void *);

/* List functions */

XMLDList *XMLDRequest_create_list(void);
XMLDRequest *XMLDRequest_add_to_list(XMLDList *);

#endif /* HAVE_XMLD_REQUEST_H */
