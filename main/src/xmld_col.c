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
#include "xmld_col.h"

/*
 * : Creates a new column.
 * val (optional): an initial value to associate to the column.
 * returns: the newly created column
 */
XMLDCol *XMLDCol_create() {
 XMLDCol *col=(XMLDCol *) malloc(sizeof(XMLDCol));
 col->val=NULL;
 return col;
}

/*
 * : Frees an allocated column
 * col: the column to free.
 */
void XMLDCol_free(XMLDCol *col) {
 XMLDCol_free_content((void *) col);
 free(col);
}

/*
 * : Frees the memory allocated by a column
 * col: the column to free
 */
void XMLDCol_free_content(void *col) {
 free(((XMLDCol *)col)->val);
}

/*
 * : Fills a column with a value.
 * col: the column to fill.
 * val: the value.
 */
void XMLDCol_fill(XMLDCol *col, char *val) {
 col->val=(char *) realloc(col->val, (strlen(val)+1)*sizeof(char));
 strcpy(col->val, val);
}

/*
 * : Creates a XMLDList of XMLDCol's.
 * returns: the newly created list.
 */
XMLDList *XMLDCol_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDCol), XMLDCol_free_content);
 return list;
}

/*
 * : Adds a new column to a XMLDList of columns.
 * list: the list to which the element is to be added.
 * val (optional) : the character value of that column.
 * cpy (optional) : whether to copy val.
 * returns: a pointer to the added XMLDCol.
 */ 
XMLDCol *XMLDCol_add_to_list(XMLDList *list) {
 XMLDCol *col=(XMLDCol *) XMLDList_add(list);
 col->val=NULL;
 return col;
}
