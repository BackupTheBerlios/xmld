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
#include "xmld_directive.h"

/*
 * : Creates a new configuration directive structure.
 * returns: the newly created directive.
 */
XMLDDirective *XMLDDirective_create() {
 return ((XMLDDirective *) malloc(sizeof(XMLDDirective)));
}

/*
 * : Frees a configuration directive structure.
 * directive: the directive structure to free.
 */
void XMLDDirective_free(XMLDDirective *directive) {
 XMLDDirective_free_content((void *) directive);
 free(directive);
};

/*
 * : Frees the memory internally allocated
 * by a configuration directive structure.
 * directive: the configuration directive structure
 * whose internal memory is to be freed.
 */
void XMLDDirective_free_content(void *directive) {
}

/*
 * : Creates a list of configuration directive
 * structures.
 * returns: the newly created list.
 */
XMLDList *XMLDDirective_create_list() {
 return XMLDList_create(sizeof(XMLDDirective), XMLDDirective_free_content);
}

/*
 * : Adds a new element to a list of configuration
 * directive structures.
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDDirective *XMLDDirective_add_to_list(XMLDList *list) {
 return ((XMLDDirective *) XMLDList_add(list));
}

/*
 * : Searches a list of configuration directives for a
 * configuration directive that has a given name.
 * list: the list to search.
 * name: the name to look for.
 * returns: a pointer to a configuration directive having
 * the given name or NULL if not found.
 */
XMLDDirective *XMLDDirective_search_list_by_name(XMLDList *list, char *name) {
 XMLDList_reset(list); 
 XMLDDirective *directive=NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDDirective *) XMLDList_curr(list))->name, name)==0) {
   directive = (XMLDDirective *) XMLDList_curr(list);
   break;
  }
 }
 return directive;
}
