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
 
#ifndef __XMLD_RESPONSE_H
#define __XMLD_RESPONSE_H

struct XMLDResponse {
 XMLDRowList *rows;
 XMLDAggrTableList *tables;
};

typedef struct XMLDResponse XMLDResponse;
typedef XMLDList XMLDResponseList;

/* Rows (and columns) manipulation functions */
XMLDResponse *XMLDResponse_create(void);
void XMLDResponse_add_row(XMLDResponse *);
void XMLDResponse_add_col(XMLDResponse *);
void XMLDResponse_fill_col(XMLDResponse *, char *);

/* Aggregate expression tables manipulation functions */
XMLDAggrTable *XMLDResponse_assoc_col_to_aggr(XMLDResponse *, XMLDExpr *, XMLDCol *);
XMLDAggrTable *XMLDResponse_search_aggr_tables_by_expr(XMLDResponse *, XMLDExpr *);
void XMLDResponse_reset_aggr(XMLDResponse *);
XMLDExpr *XMLDResponse_curr_aggr_expr(XMLDResponse *);
XMLDAggrTable *XMLDResponse_curr_aggr_table(XMLDResponse *);
/*void XMLDResponse_fill_curr_aggr(XMLDResponse *, XMLDWork *, char *(*) (XMLDWork *, XMLDExpr *, int));*/
XMLDStatus XMLDResponse_next_aggr(XMLDResponse *);
XMLDRow *XMLDResponse_curr_row(XMLDResponse *);
XMLDCol *XMLDResponse_curr_col(XMLDResponse *);
void XMLDResponse_free(XMLDResponse *);
void XMLDResponse_free_content(void *);

/* List functions */

XMLDResponseList *XMLDResoponseList_create(void);
XMLDResponse *XMLDResponseList_add(XMLDResponseList *);

#endif /* __XMLD_RESPONSE_H */
