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
#include "engine_list.h"

Status engine_list_init() {
 engine_list=Assoc_create();
 Engine *curr_engine;
 engine_list_walker = AssocWalker_create(engine_list);
 while (AssocWalker_next(engine_list_walker)) {
  (*(((Engine *) AssocWalker_get_current_data(engine_list_walker))->init)) ();
 }
 return SUCCESS;
}

Status engine_list_shutdown() {
 AssocWalker_reset(engine_list_walker);
 while (AssocWalker_next(engine_list_walker)) {
  (*(((Engine *) AssocWalker_get_current_data(engine_list_walker))->destroy)) ();
  Engine_free((Engine *) AssocWalker_get_current_data(engine_list_walker));
 }
 AssocWalker_free(engine_list_walker);
 Assoc_free(engine_list);
 return SUCCESS;
}

/*
 * : Gets the engine which is to be used
 * in processing a given file extension.
 * file: the extension of the file for which
 * an engine name is to be returned.
 * returns: if the extentsion had an engine name
 * associated to it in configurations
 * the name of that engine is returned, and
 * if not, the default engine is returned.
 */
char *engine_list_get_engine(char *file) { 
 CfgSection *engines = CfgSection_get_section(cfg_tree, "Engines", 0);
 CfgDirective *type;
 CfgValue *type_value;
 int index = 0;
 while ((type = CfgSection_get_directive(engines, "FileType", index)) != NULL) {
  type_value = CfgDirective_get_value(type, 0);
  if (strcmp(((char *) type_value->value), file) == 0) {
   type_value = CfgDirective_get_value(type, 1);
   return (char *) type_value->value;
  }
  index++;
 }
 return DEFAULT_ENGINE;
}

