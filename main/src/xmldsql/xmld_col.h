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
 
#ifndef __XMLD_COL_H
#define __XMLD_COL_H

struct XMLDCol {
 char *val;
 XMLDExpr *expr;
};

typedef struct XMLDCol XMLDCol;
typedef XMLDList XMLDColList;

XMLDCol *XMLDCol_create(void);
void XMLDCol_fill(XMLDCol *, XMLDExpr *);
void XMLDCol_free(XMLDCol *);
void XMLDCol_free_content(void *);

/* List functions */

XMLDColList *XMLDColList_create(void);
XMLDCol *XMLDColList_add(XMLDColList *);

#endif /* __XMLD_COL_H */
