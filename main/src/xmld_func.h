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

#ifndef __XMLD_FUNC_H
#define __XMLD_FUNC_H

struct XMLDFunc { /* represents a SQL function */
 char *name;
 XMLDExpr *(*func) (XMLDList *); 
 XMLDBool aggr; /* Whether it's an aggregate function */
};

#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
typedef XMLDList XMLDFuncList;

XMLDFunc *XMLDFunc_create(void);
void XMLDFunc_free(XMLDFunc *);
void XMLDFunc_free_content(void*);

/* List functions */

XMLDList *XMLDFunc_create_list(void);
XMLDFunc *XMLDFunc_add_to_list(XMLDList *);
XMLDFunc *XMLDFunc_search_list_by_name(XMLDList *, char *);

#endif /* __XMLD_FUNC_H */
