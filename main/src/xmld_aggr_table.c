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

#include "includes.h"

/*
 * : Creates a new aggregate expression table.
 * aggr (optional): the aggregate expression.
 * col_ptrs (optional): the initial XMLDList of column pointers attached to aggr.
 * returns: the newly created aggregate expression table.
 */
XMLDAggrTable *XMLDAggrTable_create() {
 XMLDAggrTable *table=(XMLDAggrTable *) malloc(sizeof(XMLDAggrTable));
 table->expr=NULL;
 table->values=NULL;
 table->aggrs=NULL;
 table->col_ptrs=NULL;
 return table;
}

/*
 * : Frees an allocated aggregate expression table.
 * table: the aggregate expression table to free.
 */
void XMLDAggrTable_free(XMLDAggrTable *table) {
 if (table != NULL) {
  XMLDAggrTable_free_content((void *)table);
  free(table);
 } 
}

/*
 * : Frees the memory internally allocated by an aggregate expression table.
 * table: the aggregate expression table whose internal memory is to be freed.
 */
void XMLDAggrTable_free_content(void *table) {
 XMLDExpr_free(((XMLDAggrTable *) table)->expr);
 XMLDList_free(((XMLDAggrTable *) table)->values);
 XMLDList_free(((XMLDAggrTable *) table)->aggrs);
 XMLDList_free(((XMLDAggrTable *) table)->col_ptrs);
}

/*
 * : Adds a new column to an aggregate expression table.
 * table: the table to whose columns the new column is to be added.
 * col: the column pointer to be added.
 */
void XMLDAggrTable_add_col(XMLDAggrTable *table, XMLDCol *col) {
 if (table->col_ptrs == NULL) {
  table->col_ptrs=XMLDList_create(sizeof(XMLDCol *), NULL);
 }
 if (table->values == NULL) {
  table->values=XMLDExprList_create();
 }
 XMLDCol **col_ptr=(XMLDCol **) XMLDList_add(table->col_ptrs);
 XMLDExpr *col_expr=XMLDExprList_add(table->values);
 XMLDExpr_copy(table->expr, col_expr); 
 *col_ptr=col;
}

/*
 * : Fills the set of columns attached to the aggregate expression table
 * with their appropriate values.
 * table: the table whose column pointers are going to be filled.
 * work: the work structure with respect to which the expression is going
 * to be determined.
 * eval_expr: the function used for expression evaluation.
 */
/*void XMLDAggrTable_fill(XMLDAggrTable *table, XMLDWork *work, char *(*eval_expr) (XMLDWork *, XMLDExpr *, int)) {
 XMLDList_reset(table->col_ptrs);
 XMLDList_reset(table->values);
 char *val;
 while (XMLDList_next(table->col_ptrs) && XMLDList_next(table->values)) {
  XMLDCol **col_ptr=(XMLDCol **) XMLDList_curr(table->col_ptrs);
  val=(*eval_expr) (work, (XMLDExpr *) XMLDList_curr(table->values), 0);
  XMLDCol_fill(*col_ptr, val);
 }
}*/

/*
 * : Creates a XMLDList of aggregate expression tables.
 * returns: the newly created list.
 */
XMLDAggrTableList *XMLDAggrTableList_create() {
 XMLDAggrTableList *list=XMLDList_create(sizeof(XMLDAggrTable), XMLDAggrTable_free_content);
 return list;
}

/*
 * : Adds a new aggregate expression table to a list of aggregate
 * expression tables.
 * list: the list to which the new element is to be added.
 * aggr (optional): the aggregate expression of the new element.
 * col_ptrs (optional): the list of column pointers of the new element.
 * returns: a pointer to the newly added aggregate expression table.
 */
XMLDAggrTable *XMLDAggrTableList_add(XMLDAggrTableList *list) {
 XMLDAggrTable *table=(XMLDAggrTable *) XMLDList_add(list);
 table->expr=NULL;
 table->values=NULL;
 table->aggrs=NULL;
 table->col_ptrs=NULL;
 return table;
}

/*
 * : Searches a list of aggregate expression tables for a particular table
 * that has a given expression as its aggregate expression.
 * list: the list to be searched.
 * aggr: the aggregate expression to search for.
 * returns: a pointer to the aggregate expression table which has an aggregate
 * expression aggr, or NULL if not found.
 */
XMLDAggrTable *XMLDAggrTableList_search_by_expr(XMLDAggrTableList *list, XMLDExpr *aggr) {
 XMLDList_reset(list);
 XMLDAggrTable *table=NULL;
 while (XMLDList_next(list)) {
  if (((XMLDAggrTable *) XMLDList_curr(list))->expr == aggr) {
   table = (XMLDAggrTable *) XMLDList_curr(list);
   break;
  }
 }
 return table;
}

/*
 * Walks through the given expression and asssociates each occurence of an 
 * aggregate function  call to an element in the aggrs field of the given 
 * XMLDAggrTable.
 */
void XMLDAggrTable_internal_assoc(XMLDAggrTable *table, XMLDExpr *expr) {
 if (expr == NULL) {
  return;
 }
 if (expr -> aggr == XMLD_FALSE) {
  return;
 }
 switch (expr->type) {
  case XMLD_OPERATION:
   XMLDAggrTable_internal_assoc(table, expr->left);
   XMLDAggrTable_internal_assoc(table, expr->right);
  break;
  case XMLD_FUNCTION:
   if (expr->func->aggr == XMLD_TRUE) {
    XMLDExprList_add(table->aggrs);
   }
   XMLDList_reset(expr->arg_list);
   while (XMLDList_next(expr->arg_list)) {
    XMLDAggrTable_internal_assoc(table, (XMLDExpr *) XMLDList_curr(expr->arg_list));
   }
  break;
  case XMLD_LIST:
   XMLDList_reset(expr->exprs);
   while (XMLDList_next(expr->exprs)) {
    XMLDAggrTable_internal_assoc(table, (XMLDExpr *) XMLDList_curr(expr->exprs));
   }
  break;
 }
}
