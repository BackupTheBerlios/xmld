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
#include "xmld_directive.h"

/*
 * : Creates a new list
 * item_size: the size of each item in the list (sizeof(item))
 * free_func: the function which is invoked on each element before
 * freeing the element array.
 * returns: the newly created list.
 */ 
XMLDList *XMLDList_create(int item_size, void (*free_func) (void *)) {
 XMLDList *list=(XMLDList *) malloc(sizeof(XMLDList));
 list->item_size=item_size;
 list->free_func=(free_func == NULL) ? default_free_func : free_func;
 list->content=NULL;
 list->last_element=NULL;
 list->curr_element=NULL;
 return list;
}

/*
 * : Frees an allocated list.
 * list: the list to free.
 */
void XMLDList_free(XMLDList *list) {
 if (list != NULL) {
  XMLDList_free_content((void *) list);
  free(list);
 } 
}

/*
 * Frees the memory internally used by a list.
 */
void XMLDList_free_content(void *list) {
 XMLDList_reset((XMLDList *) list);
 while (XMLDList_next((XMLDList *) list)) {
  (*(((XMLDList *) list)->free_func)) (XMLDList_curr((XMLDList *) list));
 }
 cfree(((XMLDList *) list)->content);
}

/*
 * : Adds an item to a list.
 * list: the list to which addition should occur.
 * returns: a pointer to the newly added item.
 */
void *XMLDList_add(XMLDList *list) {
 if (list->content == 0) {
  list->content=malloc(list->item_size);
  list->last_element=list->content;
  return list->last_element;
 }
 else {
  void *lcontent=list->content;
  list->content=realloc(list->content, (((list->last_element - lcontent) / list->item_size) + 2*list->item_size)*list->item_size);
  list->last_element=list->content + (list->last_element - lcontent + list->item_size);
  if (list->curr_element != NULL) {
   list->curr_element=list->content + (list->curr_element - lcontent);
  }
  return list->last_element;
 } 
}

/*
 * : Gets the first element in the list.
 * list: the list from which to get the first element.
 * returns: a poiner to the first element in list.
 */
void *XMLDList_first(XMLDList *list) {
 return list->content;
}

/*
 * : Gets the last element in the list.
 * list: the list from which to get the last element.
 * returns: a poiner to the last element in list.
 */
void *XMLDList_last(XMLDList *list) {
 return list->last_element;
}

/*
 * : Advances the internal list pointer by one.
 * list: the list whose pointer is to be advanced.
 * returns: whether successful.
 */
XMLDStatus XMLDList_next(XMLDList *list) {
 if (list->curr_element == list->last_element) {
  return XMLD_FAILURE;
 }
 else if (list->curr_element == NULL) {
  list->curr_element=list->content;
  return XMLD_SUCCESS;
 }
 else {
  list->curr_element+=list->item_size;
  return XMLD_SUCCESS;
 } 
}

/*
 * : Rewinds the internal list pointer by one.
 * list: the list whose pointer is to be rewinded.
 * returns: whether successful.
 */
XMLDStatus XMLDList_prev(XMLDList *list) {
 if (list->curr_element == list->content) {
  return XMLD_FAILURE;
 }
 else if (list->curr_element == NULL) {
  list->curr_element = list->last_element;
  return XMLD_SUCCESS;
 }
 else {
  list->curr_element-=list->item_size;
  return XMLD_SUCCESS;
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

/*
 * : Resets the internal list pointer, so that XMLDList_next selects the 
 * first element in the list and XMLDList_prev selects the last one.
 * list: the list to be reset.
 */
void XMLDList_reset(XMLDList *list) {
 list->curr_element=0;
}

/*
 * The default free_func used if the user provided it as NULL in 
 * the constructor
 */
void default_free_func(void *seg) {
}

/*
 * Creates a list of XMLDList's
 */
XMLDListList *XMLDListList_create() {
 XMLDListList *list=XMLDList_create(sizeof(XMLDList), XMLDList_free_content);
 return list;
}

/*
 * Add a new element to a list of XMLDList's
 */
XMLDList *XMLDListList_add(XMLDListList *list, int item_size, void (*free_func) (void *)) {
 XMLDList *curr_list=XMLDList_add(list);
 curr_list->item_size=item_size;
 curr_list->free_func=(free_func == NULL) ? default_free_func : free_func;
 curr_list->content=NULL;
 curr_list->last_element=NULL;
 curr_list->curr_element=NULL;
 return curr_list;
}
