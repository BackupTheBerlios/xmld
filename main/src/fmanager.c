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
#include "xmld_func.h"
#include "xmld_expr.h"
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
#include "cfg.h"
#include "engine_list.h"
#include "fmanager.h"

short fmanager_handle(XMLDWork *work) {
 work->res=XMLDResource_create();
 char *full_file=XMLDWork_get_full_file(work);
 work->res->engine=XMLDEngine_search_list_by_name(engine_list, cfg_get_engine(full_file));
 free(full_file);

 if (work->res->engine == NULL) {
  /* Ooooops! couldn't find an engine with the given name */
 }
 
 if (((*(work->res->engine->prepare)) (work)) == 0) {
  return -1;
 }
 else {
  return 0;
 }
}
