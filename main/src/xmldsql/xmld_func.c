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

#include "includes.h"

/*
 * : Creates a new function structure.
 * returns: the newly created function structure.
 */
XMLDFunc *XMLDFunc_create() {
 XMLDFunc *func=(XMLDFunc *) malloc(sizeof(XMLDFunc));
 func->name=NULL;
 func->func=NULL;
 func->aggr=XMLD_FALSE;
 return func;
}

/*
 * : Frees an allocated function structure.
 * func: the function structure to free.
 */
void XMLDFunc_free(XMLDFunc *func) {
 if (func != NULL) {
  XMLDFunc_free_content((void *) func);
  free(func);
 } 
}

/*
 * : Frees the memory internally allocated by a function structure.
 * func: a void pointer to the function whose memory is to be freed.
 */
void XMLDFunc_free_content(void *func) {
}

/*
 * : Creates a list of function structures.
 * returns: the newly created list.
 */
XMLDFuncList *XMLDFuncList_create() {
 XMLDFuncList *list=XMLDList_create(sizeof(XMLDFunc), XMLDFunc_free_content);
 return list;
}

/*
 * : Adds a new element to a list of function structures.
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDFunc *XMLDFuncList_add(XMLDFuncList *list) {
 XMLDFunc *func=(XMLDFunc *) XMLDList_add(list);
 func->name=NULL;
 func->func=NULL;
 func->aggr=XMLD_FALSE;
 return func;
}

/*
 * : Searches in a list of function structures for a function that
 * has a particular name.
 * list: the list to search.
 * name: the name of the function to search for.
 * returns: a pointer to the function structure that has the given
 * name or NULL if not found.
 */ 
XMLDFunc *XMLDFuncList_search_by_name(XMLDFuncList *list, char *name) {
 XMLDList_reset(list);
 XMLDFunc *func=NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDFunc *) XMLDList_curr(list))->name, name)==0) {
   func = (XMLDFunc *) XMLDList_curr(list);
   break;
  }
 }
 return func;
}