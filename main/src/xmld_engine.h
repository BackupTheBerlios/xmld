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
 void *(*init) (XMLDWork *);
 void *(*destroy) (XMLDWork *);
 int (*walk) (XMLDWork *);
 char *(*eval_expr) (XMLDWork *, XMLDExpr *);
 int (*eval_cond) (XMLDWork *, XMLDCond *);
 char *(*eval_aggr_expr) (XMLDWork *, XMLDExpr *);
};

typedef struct XMLDEngine XMLDEngine;

XMLDEngine *XMLDEngine_create(void *(*)(XMLDWork *), void *(*)(XMLDWork *), int (*)(XMLDWork *), char *(*)(XMLDWork *, XMLDExpr *), int (*) (XMLDWork *, XMLDCond *), char *(*) (XMLDWork *, XMLDExpr *));
void XMLDEngine_free(XMLDEngine *);

/* List functions */

XMLDList *XMLDEngine_create_list(void);
void XMLDEngine_add_to_list(XMLDList *, void *(*)(XMLDWork *), void *(*)(XMLDWork *), int (*)(XMLDWork *), char *(*)(XMLDWork *, XMLDExpr *), int (*) (XMLDWork *, XMLDCond *), char *(*) (XMLDWork *, XMLDExpr *));

#endif /* HAVE_XMLD_ENGINE_H */