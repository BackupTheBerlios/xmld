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
 * : Creates a new list
 * item_size: the size of each item in the list (sizeof(item))
 * returns: the newly created list.
 */ 
XMLDList *XMLDList_create(int item_size) {
 XMLDList *list=(XMLDList *) malloc(sizeof(XMLDList));
 list->item_size=item_size;
 list->content=0;
 list->last_element=0;
 list->curr_element=0;
 return list;
}

/*
 * : Frees an allocated list.
 * list: the list to free.
 */
void XMLDList_free(XMLDList *list) {
 free(list->content);
 free(list);
}

/*
 * : Adds an item to a list.
 * list: the list to which addition should occur.
 * returns: a pointer to the newly added item.
 */
void *XMLDList_add(XMLDList *list) {
 list->content=realloc(list->content, (list->last_element - list->content + 1)*list->item_size);
 list->last_element=list->content + (list->last_element - list->content);
 return list->last_element;
}

/*
 * : Points the internal list pointer to the first element in the list.
 * list: the list whose pointer is to be modified.
 */
void XMLDList_first(XMLDList *list) {
 list->curr_element = list->content;
}

/*
 * : Points the internal list pointer to the last elementin the list.
 * list: the list whose pointer is to be modified.
 */
void XMLDList_last(XMLDList *list) {
 list->curr_element = list->last_element;
}

/*
 * : Advances the internal list pointer by one.
 * list: the list whose pointer is to be advanced.
 * returns: whether successful.
 */
short XMLDList_next(XMLDList *list) {
 if (list->curr_element == list->last_element) {
  return 0;
 }
 else if (list->curr_element == 0) {
  list->curr_element = list->content;
  return 1;
 }
 else {
  list->curr_element++;
  return 1;
 } 
}

/*
 * : Rewinds the internal list pointer by one.
 * list: the list whose pointer is to be rewinded.
 * returns: whether successful.
 */
short XMLDList_prev(XMLDList *list) {
 if (list->curr_element == list->content) {
  return 0;
 }
 else if (list->curr_element == 0) {
  list->curr_element = list->last_element;
  return 1;
 }
 else {
  list->curr_element--;
  return 1;
 }
}

/*
 * : Gets the current element in the list.
 * list: the list to obtain the element from.
 * returns: a pointer to the current element in the list.
 */
void *XMLDList_curr(XMLDList *list) {
 return list->curr_element;
}
