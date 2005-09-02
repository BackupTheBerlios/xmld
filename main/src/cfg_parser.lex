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
#include "cfg_parser.tab.h"
#define YY_DECL int yylex(YYSTYPE *lvalp)
%}
%x STR
%%

#(.*)\n   ;
" " ;
"<" return '<';
">" return '>';
"/" return '/';
"\n" ;
"\""  BEGIN STR;
<STR>[^"]* {
	    lvalp->string=(char *) malloc((yyleng+1) * sizeof(char));	    
            strcpy(lvalp->string, yytext);
            return STRING;
           }
<STR>\" BEGIN INITIAL;
[A-Za-z]([A-Za-z0-9_]*) {
		lvalp->string = (char *) malloc((yyleng+1) * sizeof(char));
		strcpy(lvalp->string, yytext);
		return IDENTIFIER;
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
