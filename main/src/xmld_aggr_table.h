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

/* represents an (aggregate expression -> set of columns) association */
struct XMLDAggrTable {
 XMLDExpr *expr; /* A pointer to the original expression */
 XMLDExpr *value; /* A single copy of the original expression for internal
		     modification */
 XMLDExprList *values; /* A copy of the original for internal modification
			in a per-row way */
 XMLDExprList *aggrs; /* An expression for each aggregate function call
		         inside elements of values (identical) */
 XMLDList *col_ptrs; /* Each column here corresponds to an element of
		      values (this is a list of "XMLDCol*" */
};

typedef struct XMLDAggrTable XMLDAggrTable;
typedef XMLDList XMLDAggrTableList;

XMLDAggrTable *XMLDAggrTable_create();
void XMLDAggrTable_free(XMLDAggrTable *);
void XMLDAggrTable_free_content(void *);
void XMLDAggrTable_add_col(XMLDAggrTable *, XMLDCol *);
void XMLDAggrTable_fill(XMLDAggrTable *, XMLDWork *, char *(*) (XMLDWork *, XMLDExpr *, int));
void XMLDAggrTable_internal_assoc(XMLDAggrTable *);
void XMLDAggrTable_resolve_expr(XMLDAggrTable *, XMLDExpr *);

/* List functions */

XMLDAggrTableList *XMLDAggrTableList_create(void);
XMLDAggrTable *XMLDAggrTableList_add(XMLDAggrTableList *);
XMLDAggrTable *XMLDAggrTableList_search_by_expr(XMLDAggrTableList *, XMLDExpr *);

#endif /* __XMLD_AGGR_TABLE_h */
