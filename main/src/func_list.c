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
#include "xmlddef.h"
#include "mutils.h"
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_cond.h"
#include "xmld_col.h"
#include "xmld_row.h"
#include "xmld_aggr_table.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_request.h"
#include "xmld_response.h"
#include "xmld_resource.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"
#include "xmld_func.h"
#include "func_list.h"

XMLDStatus func_list_init() {
 func_list=XMLDFuncList_create();
 XMLDFunc *curr_func=XMLDFuncList_add(func_list);
 curr_func->name="Foo";
 curr_func->func=foo_func;
 curr_func->aggr=XMLD_FALSE;
 /* rest of function entries */
 return XMLD_SUCCESS;
}

XMLDStatus func_list_shutdown() {
 XMLDList_free(func_list);
 return XMLD_SUCCESS;
}

/* bogus function, real functions should go in the directory
 * functions */
XMLDExpr *foo_func(XMLDExprList *list) {
 return (XMLDExpr *) NULL;
}

