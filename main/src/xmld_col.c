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
 * cpy (optional): whether to copy val.
 * returns: the newly created column
 */
XMLDCol *XMLDCol_create(char *val, short cpy) {
 XMLDCol *col=(XMLDCol *) malloc(sizeof(XMLDCol));
 if (val!=NULL) {
  XMLDCol_fill(col, val, cpy);
 }
 return col;
}

/*
 * : Frees an allocated column
 * col: the column to free.
 * free_str: whether to free the internal string.
 */
void XMLDCol_free(XMLDCol *col, short free_str) {
 if (free_str) {
  free(col->val);
 }
 free(col);
}

/*
 * : Fills a column with a value.
 * col: the column to fill.
 * val: the value.
 * cpy: whether to copy val.
 */
void XMLDCol_fill(XMLDCol *col, char *val, short cpy) {
 if (cpy) {
  col->val=(char *) malloc(strlen(val)*sizeof(char));
  strcpy(col->val, val);
 }
 else {
  col->val=val;
 }
}

/*
 * : Creates a XMLDList of XMLDCol's.
 * returns: the newly created list.
 */
XMLDList *XMLDCol_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDCol));
 return list;
}

/*
 * : Adds a new column to a XMLDList of columns.
 * list: the list to which the element is to be added.
 * val (optional) : the character value of that column.
 * cpy (optional) : whether to copy val.
 * returns: a pointer to the added XMLDCol.
 */ 
XMLDCol *XMLDCol_add_to_list(XMLDList *list, char *val, short cpy) {
 XMLDCol *col=(XMLDCol *) XMLDList_add(list);
 if (val!=NULL) {
  XMLDCol_fill(col, val, cpy);
 }
 return col;
}
