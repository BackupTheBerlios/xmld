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
#include "xmld_table.h"

/*
 * : Creates a new table of objects.
 * item_size: the size in bytes of each segment in the table.
 * num: the number of segments in the table (the table never grows larger).
 * free_func: the function which is to be called on each segment before freeing.
 * returns: the newly created table.
 */ 
XMLDTable *XMLDTable_create(int item_size, int num, void (*free_func) (void *)) {
 XMLDTable *table=(XMLDTable *) malloc(sizeof(XMLDTable));
 table->list=XMLDList_create(item_size, free_func);
 table->used=0;
 table->list->content=calloc(num, item_size);
 table->list->curr_element=0;
 table->list->last_element+=num;
 table->fill=table->list->content;
 table->num=num;
 return table;
}

/*
 * : Frees an allocated table.
 * table: the table to free.
 */
void XMLDTable_free(XMLDTable *table) {
 XMLDTable_free_content((void *) table);
 free(table);
}

/*
 * : Frees the memory internally allocated by a table.
 * table: the table whose internal memory is to be allocated.
 */
void XMLDTable_free_content(void *table) {
 XMLDList_free(((XMLDTable *)table)->list);
}

/*
 * : Returns a pointer to a free segment in a table.
 * table: the table from whose segments the free segment is to be returned.
 * returns: a pointer to a free segment in the table, if all the segments were 
 * filled, the function returns NULL.
 */
void *XMLDTable_add(XMLDTable *table) {
 if (table->fill == 0) {
  return NULL;
 }
 void *ret=table->fill;
 if (table->fill == table->list->last_element) {
  table->fill=0;
 }
 else {
  table->fill++;
 }
 return ret;
}

/*
 * : Resets the internal table pointer, such that XMLDTable_next selects 
 * the first element in the table and XMLDTable_prev selects the last one.
 * table: the table to be reset.
 */
void XMLDTable_reset(XMLDTable *table) {
 XMLDList_reset(table->list);
}

/*
 * : Gets the first element in the table (used or unused!).
 * table: the table from which to get the first element.
 * returns: apointer to the first element in the table.
 */
void *XMLDTable_first(XMLDTable *table) {
 return XMLDList_first(table->list);
}

/*
 * : Gets the last element in the table (used or unused!).
 * table: the table from which to get the first element.
 * returns: a pointer to the last element in the table.
 */
void *XMLDTable_last(XMLDTable *table) {
 return XMLDList_last(table->list);
}

/*
 * : Advances the internal table pointer by one.
 * table: the table whose internal pointer is to be advanced.
 * returns: whether successful.
 */
short XMLDTable_next(XMLDTable *table) {
 return XMLDList_next(table->list);
}

/*
 * : Rewinds the internal table pointer by one.
 * table: the table whose internal pointer is to be rewinded.
 * returns: whether successful.
 */
short XMLDTable_prev(XMLDTable *table) {
 return XMLDList_prev(table->list);
}

/*
 * : Gets the current element in the table.
 * table: the table to obtain the element from.
 * returns: a pointer to the current element in the table.
 */
void *XMLDTable_curr(XMLDTable *table) {
 return XMLDList_curr(table->list);
}
