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
 
#ifndef HAVE_XMLD_ENGINE_H
#define HAVE_XMLD_ENGINE_H

struct XMLDEngine {
 char *name;
 void *(*init) (XMLDWork *);
 void *(*destroy) (XMLDWork *);
 int (*walk) (XMLDWork *);
 char *(*eval_expr) (XMLDWork *, XMLDExpr *);
 int (*eval_cond) (XMLDWork *, XMLDCond *);
 char *(*eval_aggr_expr) (XMLDWork *, XMLDExpr *);
};

#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */

XMLDEngine *XMLDEngine_create(char *);
void XMLDEngine_free(XMLDEngine *);
void XMLDEngine_free_content(void *);

/* List functions */

XMLDList *XMLDEngine_create_list(void);
XMLDEngine *XMLDEngine_add_to_list(XMLDList *, char *);
XMLDEngine *XMLDEngine_search_list_by_name(XMLDList *, char *);

#endif /* HAVE_XMLD_ENGINE_H */
