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
#include <string.h>
#include "xmld_list.h"
#include "xmld_func.h"
#include "xmld_expr.h"
#include "xmld_col.h"
#include "xmld_aggr_table.h"

/*
 * : Creates a new aggregate expression table.
 * aggr (optional): the aggregate expression.
 * col_ptrs (optional): the initial XMLDList of column pointers attached to aggr.
 * returns: the newly created aggregate expression table.
 */
XMLDAggrTable *XMLDAggrTable_create() {
 XMLDAggrTable *table=(XMLDAggrTable *) malloc(sizeof(XMLDAggrTable));
 table->aggr=NULL;
 table->col_ptrs=NULL;
 return table;
}

/*
 * : Frees an allocated aggregate expression table.
 * table: the aggregate expression table to free.
 */
void XMLDAggrTable_free(XMLDAggrTable *table) {
 XMLDAggrTable_free_content((void *)table);
 free(table);
}

/*
 * : Frees the memory internally allocated by an aggregate expression table.
 * table: the aggregate expression table whose internal memory is to be freed.
 */
void XMLDAggrTable_free_content(void *table) {
 XMLDExpr_free(((XMLDAggrTable *) table)->aggr);
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
 XMLDCol **col_ptr=(XMLDCol **) XMLDList_add(table->col_ptrs);
 *col_ptr=col;
}

/*
 * : Fills the set of columns attached to the aggregate expression table
 * with a given value.
 * table: the table whose column pointers are going to be filled.
 * val: the character value with which these column pointers are going
 * to be filled.
 */
void XMLDAggrTable_fill(XMLDAggrTable *table, char *val) {
 XMLDList_reset(table->col_ptrs);
 while (XMLDList_next(table->col_ptrs)) {
  XMLDCol **col_ptr=(XMLDCol **) XMLDList_curr(table->col_ptrs);
  XMLDCol_fill(*col_ptr, val);
 }
}

/*
 * : Creates a XMLDList of aggregate expression tables.
 * returns: the newly created list.
 */
XMLDList *XMLDAggrTable_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDAggrTable), XMLDAggrTable_free_content);
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
XMLDAggrTable *XMLDAggrTable_add_to_list(XMLDList *list) {
 XMLDAggrTable *table=(XMLDAggrTable *) XMLDList_add(list);
 table->aggr=NULL;
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
XMLDAggrTable *XMLDAggrTable_search_list_by_expr(XMLDList *list, XMLDExpr *aggr) {
 XMLDList_reset(list);
 XMLDAggrTable *table=NULL;
 while (XMLDList_next(list)) {
  if (((XMLDAggrTable *) XMLDList_curr(list))->aggr == aggr) {
   table = (XMLDAggrTable *) XMLDList_curr(list);
   break;
  }
 }
 return table;
}
