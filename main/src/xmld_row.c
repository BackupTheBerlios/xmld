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
#include "xmlddef.h"
#include "mutils.h"
#include "xmld_list.h"
#include "xmld_col.h"
#include "xmld_row.h"

/*
 * : Creates a new row.
 * list (optional): an already established list of columns.
 * returns: the newly created row.
 */
XMLDRow *XMLDRow_create() {
 XMLDRow *row=(XMLDRow *) malloc(sizeof(XMLDRow));
 row->cols=XMLDColList_create();
 return row;
}

/*
 * : Frees an allocated row.
 * row: the row to free.
 */
void XMLDRow_free(XMLDRow *row) {
 if (row != NULL) {
  XMLDRow_free_content((void *) row);
  free(row);
 } 
}

/*
 * : Frees the memory internally allocated by a row.
 * row: the row whose memory is to be freed.
 */
void XMLDRow_free_content(void *row) {
 XMLDList_free(((XMLDRow *)row)->cols);
}

/*
 * : Adds a column to a row.
 * row: the row to which the column is to be added.
 */
void XMLDRow_add_col(XMLDRow *row) {
 XMLDCol_add_to_list(row->cols);
}

/*
 * : Fills the last added column.
 * row: the row whose column is to be filled.
 * val: the value with which the row is going to be filled.
 * copy: whether to copy the given val.
 */
void XMLDRow_fill_col(XMLDRow *row, char *val) {
 XMLDCol *col=(XMLDCol *) XMLDList_last(row->cols);
 XMLDCol_fill(col, val);
}

/*
 * : Creates a XMLDList of XMLDRow's.
 * returns: the newly created list.
 */
XMLDRowList *XMLDRowList_create() {
 XMLDRowList *list=XMLDList_create(sizeof(XMLDRow), XMLDRow_free_content);
 return list;
}

/*
 * : Adds a new element to a XMLDList of XMLDRow's.
 * list: the list to which the new XMLDRow is to be added.
 * cols (optional): a pointer to an already established list of columns.
 * returns: a 
 */
XMLDRow *XMLDRowList_add(XMLDRowList *list) {
 XMLDRow *row=(XMLDRow *) XMLDList_add(list);
 row->cols=XMLDColList_create();
 return row;
}
