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
#include "../xmld_list.h"
#include "../xmld_col.h"
#include "../xmld_func.h"
#include "../xmld_expr.h"
#include "../xmld_aggr_table.h"
#include "../xmld_cond.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "../xmld_resource.h"
#include "../xmld_response.h"
#include "../xmld_request.h"
#include "../xmld_connection.h"
#include "../xmld_work.h"
#include "../xmld_engine.h"
#include "engine_xmld.h"

/* init function */
void engine_xmld_init() {
}

/* prepare function */
short engine_xmld_prepare(XMLDWork *work) {
 return 1;
}

/* cleanup function */
void engine_xmld_destroy() {
}

/* walk function */
short engine_xmld_walk(XMLDWork *work) {
 return 0;
}

/* eval_expr function */
char *engine_xmld_eval_expr(XMLDWork *work, XMLDExpr *expr) {
 char *ret=(char *) malloc(17*sizeof(char));
 strcpy(ret, "Expression Value");
 return ret;
}

/* eval_cond function */
short engine_xmld_eval_cond(XMLDWork *work, XMLDCond *cond) {
 return 1;
}

/* eval_aggr_expr function */
char *engine_xmld_eval_aggr_expr(XMLDWork *work, XMLDExpr *expr) {
 char *ret=(char *) malloc(27*sizeof(char));
 strcpy(ret, "Aggregate Expression Value");
 return ret;
}
