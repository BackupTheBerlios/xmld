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

#include "cfg_parser.tab.h"
#define YY_DECL int yylex(YYSTYPE *lvalp)
%}
%x STR
%%

" " return ' ';
"#" return '#';
"<" return '<';
">" return '>';
"/" return '/';
"\n" return '\n';
"\""  BEGIN STR;
<STR>[^"]* {
	    lvalp->string=(char *) malloc(strlen(yytext)*sizeof(char));
            strcpy(lvalp->qval, yytext);
            return STRING;
           }
<STR>\" BEGIN INITIAL;
[A-Z]([A-Z0-9_]*) {
		lvalp->dir = (char *) malloc(strlen(yytext) * sizeof(char));
		strcpy(lvalp->dir, yytext);
		return DIRECTIVE;
               }
[0-9]+ {
        lvalp->integer=atoi(yytext);
	return INTEGER;
       }
. {}

%%

int yywrap() {
 return 1;
}
