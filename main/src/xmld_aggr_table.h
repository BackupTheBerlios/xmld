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

/* represents an expression -> set of columns assocsiation */
struct XMLDAggrTable {
 XMLDExpr *aggr;
 XMLDRow *cols;
};

typedef struct XMLDAggrTable XMLDAggrTable;

XMLDAggrTable *XMLDAggrTable_create(XMLDExpr *, XMLDRow *);
void XMLDAggrTable_free_content(XMLDAggrTable *);
void XMLDAggrTable_free(XMLDAggrTable *);
void XMLDAggrTable_add_col(XMLDAggrTable *, XMLDCol *);
void XMLDAggrTable_fill(XMLDAggrTable *, char *);

#endif /* HAVE_XMLD_AGGR_TABLE_h */