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
#include "xmld_col.h"
#include "xmld_row.h"
struct XMLDExpr;
#ifndef XMLDEXPR_TYPE_DEFINED
#define XMLDEXPR_TYPE_DEFINED
 typedef struct XMLDExpr XMLDExpr;
 typedef XMLDList XMLDExprList;
#endif /* XMLDEXPR_TYPE_DEFINED */
#include "xmld_aggr_table.h"
#include "xmld_response.h"
#include "xmld_connection.h"
#include "xmld_request.h"
struct XMLDFile;
#ifndef XMLDFILE_TYPE_DEFINED
#define XMLDFILE_TYPE_DEFINED
 typedef struct XMLDFile XMLDFile;
 typedef XMLDList XMLDFileList;
#endif /* XMLDFILE_TYPE_DEFINED */
#include "xmld_work.h"
#include "xmld_cond.h"
#include "xmld_engine.h"
#include "xmld_file.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
