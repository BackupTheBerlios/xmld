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
#include "cfg_value.h"
#include "cfg_directive.h"

/*
 * : Creates a new configuration directive structure.
 * returns: the newly created directive.
 */
CfgDirective *CfgDirective_create() {
 CfgDirective *directive = (CfgDirective *) malloc(sizeof(CfgDirective));
 directive->values = NULL;
 directive->name = NULL;
 return directive;
}

/*
 * Gets the value structure of the given directive
 * if index was 0, it returns the main value.
 * Otherwise, index is the number of the parameter to return.
 */ 
CfgValue *CfgDirective_get_value(CfgDirective *directive, int index) { 
 return Assoc_get_by_index(directive, index);
}

/*
 * : Frees a configuration directive structure.
 * directive: the directive structure to free.
 */
void CfgDirective_free(CfgDirective *directive) {
 if (directive != NULL) {
  AssocWalker walker;
  walker.subject = directive->values;
  walker.curr_index = -1;
  while (AssocWalker_next(&walker)) {
   if (AssocWalker_get_current_data(&walker) != NULL) {
    CfgValue_free((CfgValue *) AssocWalker_get_current_data(&walker));
   }
  }
  Assoc_free(directive->values);
  free(directive);
 } 
};
