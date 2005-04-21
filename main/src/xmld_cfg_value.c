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
  XMLDCfgValue_free_content((void *) value);
  free(value);
 } 
};

/*
 * : Frees the memory internally allocated
 * by a configuration value structure.
 * directive: the configuration value structure
 * whose internal memory is to be freed.
 */
void XMLDCfgValue_free_content(void *value) {
 XMLDCfgValue *val = (XMLDCfgValue *) value;
 if (val->type == XMLD_CFG_STRING) {
  if ( val->value != NULL) {
   free(value->value);
  }
 }
}
