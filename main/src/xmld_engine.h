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

void XMLDEngine_free_content(XMLDEngine *);
void XMLDEngine_free(XMLDEngine *);

#endif /* HAVE_XMLD_ENGINE_H */