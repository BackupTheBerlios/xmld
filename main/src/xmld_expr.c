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
#include "xmld_list.h"
#include "xmld_func.h"
#include "xmld_expr.h"

/*
 * : Creates a new expression.
 * returns: the newly created expression.
 */
XMLDExpr *XMLDExpr_create() {
 XMLDExpr *expr=(XMLDExpr *) malloc(sizeof(XMLDExpr));
 return expr;
}

/*
 * : Frees an already allocated expression.
 * Note: this function uses the type of the expression
 * to free only the releavant fields, it's the coder's responsibility
 * to free fields not included in the current type of the expression.
 */
void XMLDExpr_free(XMLDExpr *expr) {
 XMLDExpr_free_content((void *) expr);
 free(expr);
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
   free(((XMLDExpr *)expr)->ident);
   break;
  case 3:
   XMLDFunc_free(((XMLDExpr *)expr)->func);
   XMLDList_free(((XMLDExpr *)expr)->arg_list);
   break;
  case 4:
   free(((XMLDExpr *)expr)->qval);
   break;
 }
 if (((XMLDExpr *)expr)->alias!=NULL) {
  free(((XMLDExpr *)expr)->alias);
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
 return expr;
}
