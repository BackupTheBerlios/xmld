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

void XMLDRequest_free_content(XMLDRequest *);
void XMLDRequest_free(XMLDRequest *);

#endif /* HAVE_XMLD_REQUEST_H */