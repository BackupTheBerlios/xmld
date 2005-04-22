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
  if (directive->name != NULL) {
   free(directive->name);
  }
  XMLDList_free(directive->values);
  free(directive);
 } 
};
