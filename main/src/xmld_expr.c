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

#include <stdlib.h>
#include "mutils.h"
#include <string.h>
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"

/*
 * : Creates a new expression.
 * returns: the newly created expression.
 */
XMLDExpr *XMLDExpr_create() {
 XMLDExpr *expr=(XMLDExpr *) malloc(sizeof(XMLDExpr));
 expr->aggr=0;
 expr->left=NULL;
 expr->right=NULL;
 expr->ident=NULL;
 expr->func=NULL;
 expr->arg_list=NULL;
 expr->qval=NULL;
 expr->alias=NULL;
 return expr;
}

/*
 * : Frees an already allocated expression.
 * Note: this function uses the type of the expression
 * to free only the releavant fields, it's the coder's responsibility
 * to free fields not included in the current type of the expression.
 */
void XMLDExpr_free(XMLDExpr *expr) {
 if (expr != NULL) {
  XMLDExpr_free_content((void *) expr);
  free(expr);
 } 
}

/*
 * : Frees the memory internally allocated by an expression.
 * vexpr: a void pointer the expression whose memory is to be freed.
 */
void XMLDExpr_free_content(void *expr) {
 switch(((XMLDExpr *)expr)->type) {
  case 1:
   XMLDExpr_free(((XMLDExpr *)expr)->left);
   XMLDExpr_free(((XMLDExpr *)expr)->right);
   break;
  case 2:
   cfree(((XMLDExpr *)expr)->ident);
   break;
  case 3:
   XMLDList_free(((XMLDExpr *)expr)->arg_list);
   break;
  case 4:
   cfree(((XMLDExpr *)expr)->qval);
   break;
 }
 if (((XMLDExpr *)expr)->alias != NULL) {
  cfree(((XMLDExpr *)expr)->alias);
 }
}

/*
 * : Copies an expression into another expression.
 */
void XMLDExpr_copy(XMLDExpr *src, XMLDExpr *dest) {
 dest->type=src->type;
 switch (src->type) {
  case 0:
   dest->nval=src->nval;
  break;
  case 1:
   dest->left=src->left;
   dest->right=src->right;
   dest->op=src->op;
  break;
  case 2:
   dest->ident=src->ident;
  break;
  case 3:
   dest->func=src->func;
   dest->arg_list=src->arg_list;
  break;
  case 4:   
   dest->qval=src->qval;
  break;
  case 5:
   dest->wildcard=src->wildcard;
  break;
  case 6:
   dest->fnval=src->fnval;
 }
 if (src->alias != NULL) {
  dest->alias=src->alias;
 }
}

/*
 * : Create a list of expressions.
 * returns: the newly created expression list.
 */
XMLDList *XMLDExpr_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDExpr), XMLDExpr_free_content);
 return list;
}

/*
 * : Adds a new element to a list of expressions.
 * list: the list to which the element is to be added.
 * returns: a pointer to the newly added element.
 */ 
XMLDExpr *XMLDExpr_add_to_list(XMLDList *list) {
 XMLDExpr *expr=(XMLDExpr *) XMLDList_add(list);
 expr->aggr=0;
 expr->left=NULL;
 expr->right=NULL;
 expr->ident=NULL;
 expr->func=NULL;
 expr->arg_list=NULL;
 expr->qval=NULL;
 expr->alias=NULL;
 return expr;
}
