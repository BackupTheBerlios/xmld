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
 
#ifndef HAVE_XMLD_AGGR_TABLE_TABLE_H
#define HAVE_XMLD_EXPR_TABLE_TABLE_H

struct XMLDAggrTableTable {
 XMLDAggrTable *content;
 XMLDAggrTable *last_element;
 XMLDAggrTable *curr_element; 
};

typedef struct XMLDAggrTableTable XMLDAggrTableTable;

void XMLDAggrTableTable_free_content(XMLDAggrTableTable *);
void XMLDAggrTableTable_free(XMLDAggrTableTable *);
void XMLDAggrTableTable_add(XMLDAggrTableTable *, XMLDExpr *, XMLDRow *);
void XMLDAggrTableTable_reset(XMLDAggrTableTable *);
void XMLDAggrTableTable_next(XMLDAggrTableTable *);
void XMLDAggrTableTable_prev(XMLDAggrTableTable *);
XMLDAggrTable *XMLDAggrTableTable_curr(XMLDAggrTableTable *);
XMLDAggrTable *XMLDAggrTableTable_get_table_by_expr(XMLDAggrTableTable *, XMLDExpr *);

#endif /* HAVE_XMLD_AGGR_TABLE_TABLE_H */