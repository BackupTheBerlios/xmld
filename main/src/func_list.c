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
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
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

