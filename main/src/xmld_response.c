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
#include "string.h"
#include "xmld_sockets.h"
#include "xmld_list.h"
#include "xmld_col.h"
#include "xmld_row.h"
#include "xmld_func.h"
#include "xmld_expr.h"
#include "xmld_aggr_table.h"
#include "xmld_response.h"

/*
 * : Creates a new response structure.
 * returns: the newly created structure.
 */
XMLDResponse *XMLDResponse_create() {
 XMLDResponse *resp=(XMLDResponse *) malloc(sizeof(XMLDResponse));
 resp->rows=XMLDRow_create_list();
 resp->tables=XMLDAggrTable_create_list();
 return resp;
}

/*
 * : Frees an allocated response structure.
 * resp: the response structure to free.
 */
void XMLDResponse_free(XMLDResponse *resp) {
 XMLDResponse_free_content((void *)resp);
 free(resp);
}

/*
 * : Frees the memory internally allocated by a response structure.
 * resp: the response structure whose internal memory is to be freed.
 */
void XMLDResponse_free_content(void *resp) {
 XMLDList_free(((XMLDResponse *) resp)->rows);
 XMLDList_free(((XMLDResponse *) resp)->tables);
}

/*
 * : Adds a new row of columns to a response structure.
 * resp: the response structure to which the new row is
 * to be added.
 */
void XMLDResponse_add_row(XMLDResponse *resp) {
 XMLDList_add(resp->rows);
 XMLDList_last(resp->rows);
}

/*
 * : Adds a new column to the last row in the given response structre
 * and automatically points the row's column pointer to that column.
 * resp: the mentioned response structure.
 */
void XMLDResponse_add_col(XMLDResponse *resp) {
 XMLDRow *row=(XMLDRow *) XMLDList_curr(resp->rows);
 XMLDCol_add_to_list(row->cols);
 XMLDList_last(row->cols);
}

/*
 * : Fills the last added column in the given response structure with content.
 * resp: the mentioned response structure.
 * val: the content with which the last added column is to be filled. (copied)
 */
void XMLDResponse_fill_col(XMLDResponse *resp, char *val) {
 XMLDRow *row=(XMLDRow *) XMLDList_curr(resp->rows);
 XMLDCol *col=(XMLDCol *) XMLDList_curr(row->cols);
 XMLDCol_fill(col, val);
}

/*
 * : Associates a given column to an aggregate expression table by searching for
 * the already created aggregate expression tables, if one of them matches expr
 * it's associated to the given column, and if no one of them matches expr
 * a new aggregate expression table is created, and the given column is associ-
 * ated to it.
 * resp: the response structure to be opreated on.
 * expr: the expression structure to which the given column is going to be
 * associated.
 * col: the column to be associated.
 */
void XMLDResponse_assoc_col_to_aggr(XMLDResponse *resp, XMLDExpr *expr, XMLDCol *col) {
 XMLDAggrTable *table=NULL;
 if (XMLDResponse_curr_aggr_expr(resp) == expr) {
  table=(XMLDAggrTable *) XMLDList_curr(resp->tables);
 }
 if (table == NULL) {
  table=XMLDResponse_search_aggr_tables_by_expr(resp, expr);
 }
 if (table == NULL) {
  table=XMLDAggrTable_add_to_list(resp->tables);
  table->aggr=expr;
  XMLDList_last(resp->tables);
 }
 XMLDAggrTable_add_col(table, col);
}

/*
 * : Searches the internal list of aggregate expression tables in a response 
 * structure for a table that has a given expression.
 * resp: the response structure whose list of aggregate expression tables is
 * to be searched.
 * expr: the expression for which the aggregate expression tables are going to
 * be checked.
 * returns: a pointer to the found aggregate expression table structure if
 * a match was found, and NULL if a match was not found.
 */
XMLDAggrTable *XMLDResponse_search_aggr_tables_by_expr(XMLDResponse *resp, XMLDExpr *expr) {
 return XMLDAggrTable_search_list_by_expr(resp->tables, expr);
}

/*
 * : Resets the internal list of aggregate expression tables so that a call
 * to XMLDResponse_next_aggr will select the first aggregate expression table
 * in the list.
 * resp: the response structure whose list of aggregate expression tables is
 * to be reset.
 */
void XMLDResponse_reset_aggr(XMLDResponse *resp) {
 XMLDList_reset(resp->tables);
}

/*
 * : Gets a pointer to the expression of the current aggregate expression table.
 * resp: the response structure to be operated on.
 * returns: a pointer to the expression structure of the current aggregate
 * expression table in the given response structure or NULL if no current aggregate
 * expression table was selected.
 */
XMLDExpr *XMLDResponse_curr_aggr_expr(XMLDResponse *resp) {
 XMLDAggrTable *table=(XMLDAggrTable *) XMLDList_curr(resp->tables);
 if (table!=NULL) {
  return table->aggr;
 }
 else {
  return NULL;
 }
}

/*
 * : Points the internal list pointer of the list of aggregate expression tables
 * in the given response structure to the next aggregate expression table.
 * resp: the mentioned response structure. 
 */
void XMLDResponse_next_aggr(XMLDResponse *resp) {
 XMLDList_next(resp->tables);
}

/*
 * : Fills the current aggregate expression table structure with the given value.
 * resp: the response structre to work on.
 * val: the value to fill the response structure with.
 */
void XMLDResponse_fill_curr_aggr(XMLDResponse *resp, char *val) {
 XMLDAggrTable *table=(XMLDAggrTable *) XMLDList_curr(resp->tables);
 if (table!=NULL) {
  XMLDAggrTable_fill(table, val);
 }
}

/*
 * : Creates a list of response structres.
 * returns: the newly created list.
 */
XMLDList *XMLDResponse_create_list() {
 return XMLDList_create(sizeof(XMLDResponse), XMLDResponse_free_content);
}

/*
 * : Adds a new element to a list of response structures.
 * list: the list to whose elements the new response structure is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDResponse *XMLDResponse_add_to_list(XMLDList *list) {
 XMLDResponse *resp=(XMLDResponse *) XMLDList_add(list);
 resp->rows=XMLDRow_create_list();
 resp->tables=XMLDAggrTable_create_list();
 return resp;
}

/*
 * : Flushes contents of the given response
 * structures to the given fd (should be socket)
 * resp: The response structure whose contents
 * are to be flushed.
 * fd: The file descriptor of the socket to which
 * the contents are to be flushed.
 */
void XMLDResponse_flush(XMLDResponse *resp, int fd) {
 XMLDList_reset(resp->rows);
 while (XMLDList_next(resp->rows)) {
  XMLDList_reset(((XMLDRow *) XMLDList_curr(resp->rows))->cols);
  while (XMLDList_next(((XMLDRow *) XMLDList_curr(resp->rows))->cols)) {
   xmld_socket_write(fd, ((XMLDCol *) XMLDList_curr(((XMLDRow *)XMLDList_curr(resp->rows))->cols))->val);
  }
 }
}
