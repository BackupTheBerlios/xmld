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
 
#ifndef HAVE_XMLD_COND_TABLE_H
#define HAVE_XMLD_COND_TABLE_H

struct XMLDCondTable {
 XMLDCond *content;
 XMLDCond *last_element;
 XMLDCond *curr_element; 
};

typedef struct XMLDCondTable XMLDCondTable;

void XMLDCondTable_free_content(XMLDCondTable *);
void XMLDCondTable_free(XMLDCondTable *);
void XMLDCondTable_add(XMLDCondTable *, XMLDExpr *, XMLDExpr *, short, XMLDCond *, XMLDCond *, short, short);
void XMLDCondTable_reset(XMLDCondTable *);
void XMLDCondTable_next(XMLDCondTable *);
void XMLDCondTable_prev(XMLDCondTable *);
XMLDCond *XMLDCondTable_curr(XMLDCondTable *);

#endif /* HAVE_XMLD_COND_TABLE_H */