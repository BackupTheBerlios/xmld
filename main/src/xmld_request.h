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
              * 0 = SELECT
              * 1 = SELECT with WHERE
	      * 2 = UPDATE
	      * 3 = UPDATE with WHERE
	      * 4 = DELETE (or DELETE *)
	      * 5 = DELETE with WHERE
	      * 6 = INSERT with column list
	      * 7 = INSERT with column list with WHERE
	      * 8 = INSERT with WHERE
	      * 9 = INSERT
	      * 10= USE
              */
 XMLDList *retr;  /* list of action expressions (query type dependent) */
 XMLDList *vals; /* A list of values for the above expressions  (query type dependent) */
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
