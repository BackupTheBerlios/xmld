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
#include "xmld_col.h"
#include "xmld_row.h"

/*
 * : Creates a new row.
 * list (optional): an already established list of columns.
 * returns: the newly created row.
 */
XMLDRow *XMLDRow_create(XMLDList *cols) {
 XMLDRow *row=(XMLDRow *) malloc(sizeof(XMLDRow));
 if (cols==NULL) {
  row->cols=XMLDCol_create_list();
 }
 else {
  row->cols=cols;	 
 }
 return row;
}

/*
 * : Frees an allocated row.
 * row: the row to free.
 */
void XMLDRow_free(XMLDRow *row) {
 XMLDList_free(row->cols);
 free(row);
}

/*
 * : Adds a column to a row.
 * row: the row to which the column is to be added.
 */
void XMLDRow_add_col(XMLDRow *row) {
 XMLDCol_add_to_list(row->cols, NULL, 0);
}

/*
 * : Fills the last added column.
 * row: the row whose column is to be filled.
 * val: the value with which the row is going to be filled.
 * copy: whether to copy the given val.
 */
void XMLDRow_fill_col(XMLDRow *row, char *val, short cpy) {
 XMLDCol *col=(XMLDCol *) XMLDList_last(row->cols);
 XMLDCol_fill(col, val, cpy);
}
