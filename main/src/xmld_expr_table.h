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
 
#ifndef HAVE_XMLD_EXPR_TABLE_H
#define HAVE_XMLD_EXPR_TABLE_H

struct XMLDExprTable {
 XMLDExpr *content;
 XMLDExpr *last_element;
 XMLDExpr *curr_element; 
};

typedef struct XMLDExprTable XMLDExprTable;

XMLDExprTable *XMLDExprTable_create(void);
void XMLDExprTable_free_content(XMLDExprTable *);
void XMLDExprTable_free(XMLDExprTable *);
void XMLDExprTable_add(XMLDExprTable *, short, short, int, XMLDExpr *, XMLDExpr *, short, char *, XMLDFunc *, XMLDExprTable *, char *, short, char *);
void XMLDExprTable_reset(XMLDExprTable *);
void XMLDExprTable_next(XMLDExprTable *);
void XMLDExprTable_prev(XMLDExprTable *);
XMLDExpr *XMLDExprTable_curr(XMLDExprTable *);

#endif /* HAVE_XMLD_EXPR_TABLE_H */