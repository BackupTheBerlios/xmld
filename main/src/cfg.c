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
#include "xmld_list.h"
#include "xmld_directive.h"
#include "cfg_parser.h"
#include "cfg.h"

/*
 * : Initiates the configuration manager.
 * returns: whether successful.
 */
XMLDStatus cfg_init() {
 cfg_tree=NULL;
 if (cfg_parser_parse() == XMLD_FAILURE) {
  return XMLD_FAILURE;
 }
 col_sep=*((char *) cfg_get("response.col_sep"));
 col_sep_enc=((char *) cfg_get("response.col_sep_enc"));
 row_sep=*((char *) cfg_get("response.row_sep"));
 row_sep_enc=((char *) cfg_get("response.row_sep_enc"));
 down_level=*((char *) cfg_get("response.down_level"));
 down_level_enc=((char *) cfg_get("response.down_level_enc"));
 up_level=*((char *) cfg_get("response.up_level"));
 up_level_enc=((char *) cfg_get("response.up_level_enc"));
 document_root=((char *) cfg_get("xmld.document_root"));
 return XMLD_SUCCESS;
}

/*
 * : Gets the value of a particular configuration
 * directive.
 * key: the name of the configuration directive
 * whose value is to be aquired.
 * returns: a void pointer to the value of the given
 * directive or NULL if the directive was not found
 * or if an error occured during retrieval.
 */ 
void *cfg_get(char *key) {
 XMLDDirective *dir=XMLDDirectiveList_search_by_name(cfg_tree, key);
 if (dir == NULL) {
  return (void *) NULL;
 }
 switch (dir->type) {
  case XMLD_DIR_INT:
   return (void *) &dir->value.int_value;
  case XMLD_DIR_INTARR:
   return (void *) dir->value.int_array_value;
  case XMLD_DIR_STR:
   return (void *) dir->value.string_value;
  case XMLD_DIR_STRARR:
   return (void *) dir->value.string_array_value;
  case XMLD_DIR_CHAR:
   return (void *) &dir->value.char_value;
  default:
   return (void *) NULL;
 }
}

/*
 * : Gets the engine which is to be used
 * in processing a given file.
 * file: the name of the file for which
 * an engine name is to be returned.
 * returns: if the file had an engine name
 * associated to it in configurations
 * the name of that engine is returned, and
 * if not, NULL is returned (MIME checking
 * should be used to determine the engine).
 */

/* ATTENTION: cfg_get_engine, whensever written
 * must free the string returned from mfigure_get
 * _mime explicitly if it uses it
 * ATTENTION again: it must check for mime compaitability
 * using is_valid_mime and try to reuse mfigure_get_mime
 * if it was used. If checking fails return NULL.
 */
char *cfg_get_engine(char *file) { 
 return "Engine-XMLD";
}

/*
 * : Associates a particular file name to
 * a particular engine.
 * file: the file to be associated.
 * engine: the engine to be linked to the file.
 */ 
void cfg_set_engine(char *file, char *engine) { 
}

/*
 * : Gets the default engine for the given _mime_
 * returns: the name of the mentioned engine.
 */
char *cfg_get_mime_engine(char *mime) { 
 return (char *) NULL;
}

/*
 * : Sets the default engine for a given _mime_.
 */
void cfg_set_mime_engine(char *mime, char *engine) {
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
XMLDStatus cfg_shutdown() {
 return cfg_parser_clean();
}
 
