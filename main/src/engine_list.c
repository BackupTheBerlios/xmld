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
#include <string.h>
#include "xmld_list.h"
#include "xmld_col.h"
#include "xmld_row.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_aggr_table.h"
#include "xmld_cond.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_resource.h"
#include "xmld_response.h"
#include "xmld_request.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"
#include "engine_list.h"

#ifdef USE_ENGINE_XMLD
#include "engine_xmld/engine_xmld.h"
#endif /* USE_ENGINE_XMLD */
 
short engine_list_init() {
 engine_list=XMLDEngine_create_list();
 XMLDEngine *curr_engine;
#ifdef USE_ENGINE_XMLD
 curr_engine=XMLDEngine_add_to_list(engine_list, "Engine-XMLD");
 curr_engine->init=engine_xmld_init;
 curr_engine->prepare=engine_xmld_prepare;
 curr_engine->cleanup=engine_xmld_cleanup;
 curr_engine->destroy=engine_xmld_destroy;
 curr_engine->eval_expr=engine_xmld_eval_expr;
 curr_engine->eval_cond=engine_xmld_eval_cond;
#endif /* USE_ENGINE_XMLD */
 XMLDList_reset(engine_list);
 while (XMLDList_next(engine_list)) {
  (*(((XMLDEngine *) XMLDList_curr(engine_list))->init)) ();
 }
 
 return 1;
}

short engine_list_shutdown() {
 XMLDList_reset(engine_list);
 while (XMLDList_next(engine_list)) {
  (*(((XMLDEngine *) XMLDList_curr(engine_list))->destroy)) ();
 }
 XMLDList_free(engine_list);
 return 1;
}
