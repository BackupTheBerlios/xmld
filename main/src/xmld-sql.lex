%{
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
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
#include "xmld_errors.h"
#include "func_list.h"
#include "xmld-sql.tab.h"
#include "qp.h"
#define YY_DECL int yylex(YYSTYPE *lvalp)
%}
%x STR
%%

"select" return SELECT;
"disconnect" return DISCONNECT;
"from" return FROM;
"where" return WHERE;
"update" return UPDATE;
"set" return SET;
"delete" return DELETE;
"insert" return INSERT;
"into" return INTO;
"values" return VALUES;
"use" return USE;
"as" return AS;
"and" return AND;
"or" return OR;
"<>" return NE;
"<=" return LE;
">=" return GE;
"like" return LIKE;
"between" return BETWEEN;
"not between" return NBETWEEN;
":" return ':';
"," return ',';
"!" return '!';
"=" return '=';
"\""  BEGIN STR;
<STR>[^"]* {
	   lvalp->qval=(char *) malloc(strlen(yytext)*sizeof(char));
           strcpy(lvalp->qval, yytext);
           return QVAL;
          }
<STR>\" BEGIN INITIAL;
[A-Z_]([A-Z0-9_]*) {
                   lvalp->qval=(char *) malloc((strlen(yytext)+1)*sizeof(char));
		   strcpy(lvalp->qval, yytext);
		   return IDENTIFIER;
                  }
([(]{1})(("text"|"tagname"){1})([)]{1}) {
                                         lvalp->qval=(char *) malloc((strlen(yytext)+1)*sizeof(char));
		                         strcpy(lvalp->qval, yytext);
		                         return IDENTIFIER;
                                        }
[0-9]+ {
        lvalp->num=atol(yytext);
	return NUM;
       }
[*] return '*';
[@] return '@';
. {}

%%

int yywrap() {
 return 1;
}
