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

#include "includes.h"
#include "func_list.h"
#include "xmld-sql.tab.h"
#include "qp.h"
#define YY_DECL int yylex(YYSTYPE *lvalp)
%}
%x STR
%%

"select" return SELECT;
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
"<" return '<';
">" return '>';
"like" return LIKE;
"between" return BETWEEN;
"not between" return NBETWEEN;
":" return ':';
"," return ',';
"!" return '!';
"=" return '=';
"~" return '~';
"." return '.';
"*" return '*';
"@" return '@';
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
([[]{1})(("text"|"tagname"){1})([]]{1}) {
                                         lvalp->qval=(char *) malloc((strlen(yytext)+1)*sizeof(char));
		                         strcpy(lvalp->qval, yytext);
		                         return SPECIAL_IDENTIFIER;
                                        }
[0-9]+"."[0-9]+ {
                 lvalp->fnum=atof(yytext);
		 return FNUM;
                }
[0-9]+ {
        lvalp->num=atoi(yytext);
	return NUM;
       }
. {}

%%

int yywrap() {
 return 1;
}
