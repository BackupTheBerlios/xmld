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

/* Rows (and columns) manipulation functions */
XMLDResponse *XMLDResponse_create(void);
void XMLDResponse_add_row(XMLDResponse *);
void XMLDResponse_add_col(XMLDResponse *);
void XMLDResponse_fill_col(XMLDResponse *, char *);

/* Aggregate expression tables manipulation functions */
void XMLDResponse_assoc_col_to_aggr(XMLDResponse *, XMLDExpr *, XMLDCol *);
XMLDAggrTable *XMLDResponse_search_aggr_tables_by_expr(XMLDResponse *, XMLDExpr *);
void XMLDResponse_reset_aggr(XMLDResponse *);
XMLDExpr *XMLDResponse_curr_aggr_expr(XMLDResponse *);
void XMLDResponse_fill_curr_aggr(XMLDResponse *, char *);
void XMLDResponse_next_aggr(XMLDResponse *);
void XMLDResponse_free(XMLDResponse *);
void XMLDResponse_free_content(void *);

/* List functions */

XMLDList *XMLDResoponse_create_list(void);
XMLDResponse *XMLDResponse_add_to_list(XMLDList *);

#endif /* HAVE_XMLD_RESPONSE_H */
