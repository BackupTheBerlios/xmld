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


