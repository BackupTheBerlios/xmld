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
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "func_list.h"

short func_list_init() {
 func_list=XMLDFunc_create_list();
 XMLDFunc *curr_func=XMLDFunc_add_to_list(func_list);
 curr_func->name="Foo";
 curr_func->func=foo_func;
 curr_func->aggr=0;
 /* rest of function entries */
 return 1;
}

short func_list_shutdown() {
 XMLDList_free(func_list);
 return 1;
}

/* bogus function, real functions should go here  */
XMLDExpr *foo_func(XMLDList *list) {
 return (XMLDExpr *) NULL;
}

