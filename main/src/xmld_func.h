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

#ifndef HAVE_XMLD_FUNC_H
#define HAVE_XMLD_FUNC_H

struct XMLDFunc {
 char *name;
 void *(*func) (XMLDExprTable *);
 short aggr; /*
               * 1 = Aggregate function
               * 0 = Scalar function
               */
};

typedef struct XMLDFunc XMLDFunc;

XMLDFunc *XMLDFunc_create(char *, void *(*) (XMLDExprTable *), short);
void XMLDFunc_free(XMLDFunc *);

/* List functions */

XMLDList *XMLDFunc_create_list(void);
void XMLDFunc_add_to_list(XMLDList *, char *, void *(*) (XMLDExprTable *), short);
XMLDFunc *XMLDFunc_search_list_by_name(XMLDList *, char *);

#endif /* HAVE_XMLD_FUNC_H */
