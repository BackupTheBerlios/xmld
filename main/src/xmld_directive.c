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
#include "mutils.h"
#include "xmld_list.h"
#include "xmld_directive.h"

/*
 * : Creates a new configuration directive structure.
 * returns: the newly created directive.
 */
XMLDDirective *XMLDDirective_create() {
 XMLDDirective *dir=(XMLDDirective *) malloc(sizeof(XMLDDirective));
 dir->name=NULL;
 dir->type=XMLD_DIR_STR;
 dir->value.string_value=NULL;
 return dir;
}

/*
 * : Frees a configuration directive structure.
 * directive: the directive structure to free.
 */
void XMLDDirective_free(XMLDDirective *directive) {
 if (directive != NULL) {
  XMLDDirective_free_content((void *) directive);
  free(directive);
 } 
};

/*
 * : Frees the memory internally allocated
 * by a configuration directive structure.
 * directive: the configuration directive structure
 * whose internal memory is to be freed.
 */
void XMLDDirective_free_content(void *directive) {
 XMLDDirective *dir=(XMLDDirective *) directive;
 if (dir->type == XMLD_DIR_INTARR) {
  cfree(dir->value.int_array_value);
 }
 else if (dir->type == XMLD_DIR_STR) {
  cfree(dir->value.string_value);
 }
 else if (dir->type == XMLD_DIR_STRARR) {
  char **ptr=dir->value.string_array_value;
  while (ptr != NULL) {
   cfree(*ptr);
   ptr++;
  }
  cfree(dir->value.string_array_value);
 }
 if (dir->type != XMLD_DIR_ERROR) {
  cfree(dir->name);
 }
}

/*
 * : Creates a list of configuration directive
 * structures.
 * returns: the newly created list.
 */
XMLDDirectiveList *XMLDDirectiveList_create() {
 return XMLDList_create(sizeof(XMLDDirective), XMLDDirective_free_content);
}

/*
 * : Adds a new element to a list of configuration
 * directive structures.
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDDirective *XMLDDirectiveList_add_(XMLDDirectiveList *list) {
 XMLDDirective *dir=(XMLDDirective *) XMLDList_add(list);
 dir->name=NULL;
 dir->type=XMLD_DIR_STR;
 dir->value.string_value=NULL;
 return dir;
}

/*
 * : Searches a list of configuration directives for a
 * configuration directive that has a given name.
 * list: the list to search.
 * name: the name to look for.
 * returns: a pointer to a configuration directive having
 * the given name or NULL if not found.
 */
XMLDDirective *XMLDDirectiveList_search_by_name(XMLDDirectiveList *list, char *name) {
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
