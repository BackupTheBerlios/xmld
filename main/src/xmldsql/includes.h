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

#include "../includes.h"
struct XMLDExpr;
#ifndef XMLDEXPR_TYPE_DEFINED
#define XMLDEXPR_TYPE_DEFINED
 typedef struct XMLDExpr XMLDExpr;
 typedef XMLDList XMLDExprList;
#endif /* XMLDEXPR_TYPE_DEFINED */
#include "xmld_col.h"
#include "xmld_row.h"
#include "xmld_aggr_table.h"
#include "xmld_response.h"
#include "xmld_request.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "twalker.h"
#include "func_list.h"
#include "resptrans.h"
