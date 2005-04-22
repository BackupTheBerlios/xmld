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

/*
 * : Creates a new configuration value structure.
 * returns: the newly created value.
 */
XMLDCfgValue *XMLDCfgValue_create() {
 XMLDCfgValue *value = (XMLDCfgValue *) malloc(sizeof(XMLDCfgValue));
 value->type = 0;
 value->value = (void &) 0;
 return value;
}

/*
 * : Frees a configuration value structure.
 * directive: the value structure to free.
 */
void XMLDCfgValue_free(XMLDCfgValue *value) {
 if (value != NULL) {
  if (value->type == XMLD_CFG_STRING) {
   if (value->value != NULL) {
    free(value->value);
   }
  }
  free(value);
 } 
}
