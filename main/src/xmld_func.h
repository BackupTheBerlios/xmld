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

struct XMLDFunc { /* represents a SQL function */
 char *name;
 void *(*func) (XMLDList *);
 short aggr; /*
               * 1 = Aggregate function
               * 0 = Scalar function
               */
};

typedef struct XMLDFunc XMLDFunc;

XMLDFunc *XMLDFunc_create(void);
void XMLDFunc_free(XMLDFunc *);
void XMLDFunc_free_content(void*);

/* List functions */

XMLDList *XMLDFunc_create_list(void);
XMLDFunc *XMLDFunc_add_to_list(XMLDList *);
XMLDFunc *XMLDFunc_search_list_by_name(XMLDList *, char *);

#endif /* HAVE_XMLD_FUNC_H */
