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

#ifdef USE_ENGINE_XMLD
#include "engine_xmld/engine_xmld.h"
#endif /* USE_ENGINE_XMLD */
 
XMLDStatus engine_list_init() {
 engine_list=XMLDAssoc_create();
 XMLDEngine *curr_engine;
#ifdef USE_ENGINE_XMLD
 curr_engine=XMLDEngine_create();
 curr_engine->is_valid_mime=engine_xmld_is_valid_mime;
 curr_engine->init=engine_xmld_init;
 curr_engine->prepare=engine_xmld_prepare;
 curr_engine->cleanup=engine_xmld_cleanup;
 curr_engine->walk=engine_xmld_walk;
 curr_engine->destroy=engine_xmld_destroy;
 curr_engine->get_attribute_type=engine_xmld_get_attribute_type;
 curr_engine->get_attribute_length=engine_xmld_get_attribute_length;
 curr_engine->get_attribute=engine_xmld_get_attribute;
 curr_engine->get_text_type=engine_xmld_get_text_type;
 curr_engine->get_text_length=engine_xmld_get_text_length;
 curr_engine->get_text=engine_xmld_get_text;
 curr_engine->get_tagname=engine_xmld_get_tagname;
 curr_engine->reset_element=engine_xmld_reset_element;
 curr_engine->next_attribute=engine_xmld_next_attribute;
 curr_engine->get_curr_attribute_type=engine_xmld_get_curr_attribute_type;
 curr_engine->get_curr_attribute_length=engine_xmld_get_curr_attribute_length;
 curr_engine->get_curr_attribute_name=engine_xmld_get_curr_attribute_name;
 curr_engine->get_curr_attribute_value=engine_xmld_get_curr_attribute_value;
 XMLDAssoc_add(engine_list, "Engine-XMLD", curr_engine);
#endif /* USE_ENGINE_XMLD */
 engine_list_walker = XMLDAssocWalker_create(engine_list);
 while (XMLDAssocWalker_next(engine_list_walker)) {
  (*(((XMLDEngine *) XMLDAssocWalker_get_current_data(engine_list_walker))->init)) ();
 }
 return XMLD_SUCCESS;
}

XMLDStatus engine_list_shutdown() {
 XMLDAssocWalker_reset(engine_list_walker);
 while (XMLDAssocWalker_next(engine_list_walker)) {
  (*(((XMLDEngine *) XMLDAssocWalker_get_current_data(engine_list_walker))->destroy)) ();
  XMLDEngine_free((XMLDEngine *) XMLDAssocWalker_get_current_data(engine_list_walker));
 }
 XMLDAssocWalker_free(engine_list_walker);
 XMLDAssoc_free(engine_list);
 return XMLD_SUCCESS;
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

/* ATTENTION: it must check for mime compaitability
 * using is_valid_mime and try to reuse mfigure_get_mime
 * if it was used. If checking fails return NULL.
 */
char *engine_list_get_engine(char *file) { 
 return "Engine-XMLD";
}

