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
 
#ifndef HAVE_XMLD_RESPONSE_H
#define HAVE_XMLD_RESPONSE_H

struct XMLDResponse {
 XMLDList *rows;
 XMLDList *tables;
};

typedef struct XMLDResponse XMLDResponse;

XMLDResponse *XMLDResponse_create(void);
void XMLDResponse_add_row(XMLDResponse *);
void XMLDResponse_fill_col(XMLDResponse *, char *); /*
						     * Note the given string isn't copied.
						     */
void XMLDResponse_add_col(XMLDResponse *);
void XMLDResponse_add_aggr(XMLDResponse *, XMLDExpr *);
void XMLDResponse_aggr_reset(XMLDResponse *); /* Resets _curr_table_ */
XMLDExpr *XMLDResponse_curr_aggr(XMLDResponse *); /* return the current aggr expr */
void XMLDResponse_fill_aggr(XMLDResponse *, char *);
void XMLDResponse_aggr_next(XMLDResponse *);

/* List functions */

XMLDList *XMLDResoponse_create_list(void);
void XMLDResponse_add_to_list(XMLDList *);

#endif /* HAVE_XMLD_RESPONSE_H */
