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
 XMLDExprTable *retr;  /* the list of expressions to be
		                    * retrieved 
		                    */
 XMLDCondTable *where; /* A condition for each level */
};

typedef struct XMLDRequest XMLDRequest;

XMLDRequest *XMLDRequest_create(char *, short, XMLDList *, XMLDList *);
void XMLDRequest_free(XMLDRequest *);

/* List functions */

XMLDList *XMLDRequest_create_list(void);
void XMLDRequest_add_to_list(XMLDList *, char *, short, XMLDList *, XMLDList *);

#endif /* HAVE_XMLD_REQUEST_H */