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

#ifndef HAVE_XMLD_AGGR_TABLE_h
#define HAVE_XMLD_AGGR_TABLE_h

/* represents an (aggregate expression -> set of columns) assocsiation */
struct XMLDAggrTable {
 XMLDExpr *aggr;
 XMLDList *col_ptrs; /* list of "XMLDCol*" */
};

typedef struct XMLDAggrTable XMLDAggrTable;

XMLDAggrTable *XMLDAggrTable_create(XMLDExpr *, XMLDList *);
void XMLDAggrTable_free(XMLDAggrTable *);
void XMLDAggrTable_free_content(void *);
void XMLDAggrTable_add_col(XMLDAggrTable *, XMLDCol *);
void XMLDAggrTable_fill(XMLDAggrTable *, char *);

/* List functions */

XMLDList *XMLDAggrTable_create_list(void);
XMLDAggrTable *XMLDAggrTable_add_to_list(XMLDList *, XMLDExpr *, XMLDList *);
XMLDAggrTable *XMLDAggrTable_search_list_by_expr(XMLDList *, XMLDExpr *);

#endif /* HAVE_XMLD_AGGR_TABLE_h */
