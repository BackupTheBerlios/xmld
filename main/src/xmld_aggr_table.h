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

#ifndef __XMLD_AGGR_TABLE_h
#define __XMLD_AGGR_TABLE_h

/* represents an (aggregate expression -> set of columns) assocsiation */
struct XMLDAggrTable {
 XMLDExpr *aggr;
 XMLDExpr *value;
 XMLDList *col_ptrs; /* list of "XMLDCol*" */
};

typedef struct XMLDAggrTable XMLDAggrTable;
typedef XMLDList XMLDAggrTableList;

XMLDAggrTable *XMLDAggrTable_create();
void XMLDAggrTable_free(XMLDAggrTable *);
void XMLDAggrTable_free_content(void *);
void XMLDAggrTable_add_col(XMLDAggrTable *, XMLDCol *);
void XMLDAggrTable_fill(XMLDAggrTable *, char *);

/* List functions */

XMLDAggrTableList *XMLDAggrTableList_create(void);
XMLDAggrTable *XMLDAggrTableList_add(XMLDAggrTableList *);
XMLDAggrTable *XMLDAggrTableList_search_by_expr(XMLDAggrTableList *, XMLDExpr *);

#endif /* __XMLD_AGGR_TABLE_h */
