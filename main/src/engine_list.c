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
 engine_list=XMLDEngineList_create();
 XMLDEngine *curr_engine;
#ifdef USE_ENGINE_XMLD
 curr_engine=XMLDEngineList_add(engine_list, "Engine-XMLD");
 curr_engine->init=engine_xmld_init;
 curr_engine->prepare=engine_xmld_prepare;
 curr_engine->cleanup=engine_xmld_cleanup;
 curr_engine->walk=engine_xmld_walk;
 curr_engine->destroy=engine_xmld_destroy;
 curr_engine->get_attribute_type=engine_xmld_get_attribute_type;
 curr_engine->get_attribute=engine_xmld_get_attribute;
 curr_engine->get_text_type=engine_xmld_get_text_type;
 curr_engine->get_text=engine_xmld_get_text;
 curr_engine->get_tagname=engine_xmld_get_tagname;
 curr_engine->reset_element=engine_xmld_reset_element;
 curr_engine->next_attribute=engine_xmld_next_attribute;
 curr_engine->get_curr_attribute_type=engine_xmld_get_curr_attribute_type;
 curr_engine->get_curr_attribute_name=engine_xmld_get_curr_attribute_name;
 curr_engine->get_curr_attribute_value=engine_xmld_get_curr_attribute_value;
#endif /* USE_ENGINE_XMLD */
 XMLDList_reset(engine_list);
 while (XMLDList_next(engine_list)) {
  (*(((XMLDEngine *) XMLDList_curr(engine_list))->init)) ();
 }
 
 return XMLD_SUCCESS;
}

XMLDStatus engine_list_shutdown() {
 XMLDList_reset(engine_list);
 while (XMLDList_next(engine_list)) {
  (*(((XMLDEngine *) XMLDList_curr(engine_list))->destroy)) ();
 }
 XMLDList_free(engine_list);
 return XMLD_SUCCESS;
}
