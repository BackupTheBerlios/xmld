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
#include "xmld_list.h"
#include "xmld_cfg_value.h"
#include "xmld_cfg_directive.h"

/*
 * : Creates a new configuration directive structure.
 * returns: the newly created directive.
 */
XMLDCfgDirective *XMLDCfgDirective_create() {
 XMLDCfgDirective *directive = (XMLDCfgDirective *) malloc(sizeof(XMLDCfgDirective));
 directive->name = NULL;
 directive->values = NULL;
 return directive;
}

/*
 * Gets the value structure of the given directive
 * if index was 0, it returns the main value.
 * Otherwise, index is the number of the parameter to return.
 */ 
XMLDCfgValue *XMLDCfgDirective_get_value(XMLDCfgDirective *directive, int index) {
 int num = 0;
 while (XMLDList_next(directive->values)) {
  if (num == index) {
   return (XMLDCfgValue *) XMLDList_curr(directive->values);
  }
  num++;
 }
 return NULL;
}

/*
 * : Frees a configuration directive structure.
 * directive: the directive structure to free.
 */
void XMLDCfgDirective_free(XMLDCfgDirective *directive) {
 if (directive != NULL) {
  XMLDCfgDirective_free_content((void *) directive);
  free(directive);
 } 
};

/*
 * : Frees the memory internally allocated
 * by a configuration directive structure.
 * directive: the configuration directive structure
 * whose internal memory is to be freed.
 */
void XMLDCfgDirective_free_content(void *directive) {
 XMLDCfgDirective *dir = (XMLDCfgDirective *) directive;
 if (dir->name != NULL) {
  free(dir->name);
 } 
 XMLDList_free(dir->values);
}

/*
 * : Creates a list of configuration directive
 * structures.
 * returns: the newly created list.
 */
XMLDCfgDirectiveList *XMLDCfgDirectiveList_create() {
 return XMLDList_create(sizeof(XMLDCfgDirective), XMLDCfgDirective_free_content);
}

/*
 * : Adds a new element to a list of configuration
 * directive structures.
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDCfgDirective *XMLDCfgDirectiveList_add(XMLDCfgDirectiveList *list) {
 XMLDCfgDirective *directive = (XMLDCfgDirective *) XMLDList_add(list);
 directive->name = NULL;
 directive->values = NULL;
 return directive;
}

/*
 * : Searches a list of configuration directives for a
 * configuration directive that has a given name.
 * list: the list to search.
 * name: the name to look for.
 * returns: a pointer to a configuration directive having
 * the given name or NULL if not found.
 */
XMLDCfgDirective *XMLDCfgDirectiveList_search_by_name(XMLDCfgDirectiveList *list, char *name) {
 XMLDList_reset(list);
 XMLDCfgDirective *directive = NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDCfgDirective *) XMLDList_curr(list))->name, name)==0) {
   directive = (XMLDCfgDirective *) XMLDList_curr(list);
   break;
  }
 }
 return directive;
}
