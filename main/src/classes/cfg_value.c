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

/*
 * : Creates a new configuration value structure.
 * returns: the newly created value.
 */
CfgValue *CfgValue_create() {
 CfgValue *value = (CfgValue *) malloc(sizeof(CfgValue));
 value->type = 0;
 value->value = (void &) 0;
 return value;
}

/*
 * : Frees a configuration value structure.
 * directive: the value structure to free.
 */
void CfgValue_free(CfgValue *value) {
 if (value != NULL) {
  if (value->type == CFG_STRING) {
   if (value->value != NULL) {
    free(value->value);
   }
  }
  free(value);
 } 
}
