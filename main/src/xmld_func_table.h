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
 
#ifndef HAVE_XMLD_FUNC_TABLE_H
#define HAVE_XMLD_FUNC_TABLE_H

struct XMLDFuncTable {
 XMLDFunc *content;
 XMLDFunc *last_element;
 XMLDFunc *curr_element; 
};

typedef struct XMLDFuncTable XMLDFuncTable;

void XMLDFuncTable_free_content(XMLDFuncTable *);
void XMLDFuncTable_free(XMLDFuncTable *);
void XMLDFuncTable_add(XMLDFuncTable *, char *, void (*) (XMLDExprTable *), short);
void XMLDFuncTable_reset(XMLDFuncTable *);
void XMLDFuncTable_next(XMLDFuncTable *);
void XMLDFuncTable_prev(XMLDFuncTable *);
XMLDFunc *XMLDFuncTable_curr(XMLDFuncTable *);
XMLDFunc *XMLDFuncTable_get_func_by_name(XMLDFuncTable *, char *);

#endif /* HAVE_XMLD_FUNC_TABLE_H */
