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
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_cond.h"

/*
 * : Creates a new condition structure.
 * returns: the newly created condition structure.
 */
XMLDCond *XMLDCond_create() {
 XMLDCond *cond = (XMLDCond *) malloc(sizeof(XMLDCond));
 return cond;
}

/*
 * : Frees a condition structure.
 * cond: the condition structure to free.
 */
void XMLDCond_free(XMLDCond *cond) {
 XMLDCond_free_content((void *) cond);
 free(cond);
}

/*
 * : Frees the memory internally allocated by a condition 
 * structure.
 * cond: the condition whose internal memory is to be freed.
 */
void XMLDCond_free_content(void *cond) {
 if (((XMLDCond *) cond)->type == 0) {
  XMLDExpr_free(((XMLDCond *) cond)->left);
  XMLDExpr_free(((XMLDCond *) cond)->right);
 }
 else if (((XMLDCond *) cond)->type ==1) {
  XMLDCond_free(((XMLDCond *) cond)->cleft);
  XMLDCond_free(((XMLDCond *) cond)->cright);
 }
}

/*
 * : Creates a list of condition structures.
 * returns: the newly created list.
 */
XMLDList *XMLDCond_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDCond), XMLDCond_free_content);
 return list;
}

/*
 * : Adds a new condition structure to a list of condition 
 * structures.
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDCond *XMLDCond_add_to_list(XMLDList *list) {
 XMLDCond *cond=(XMLDCond *) XMLDList_add(list);
 return cond;
}
