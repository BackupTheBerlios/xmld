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
#include "xmld_types.h"
#include "xmld-sql.tab.h"
int yyparse(void);
%}

%%

"\"[a-z0-9]*\""|"\'[a-z0-9]*\'" {
                                 realloc(yylval.str, strlen(yytext)*sizeof(char));
				 strcpy(yylval.str, yytext);
				 return QUOTED_VAL;
				} 
"select" return SELECT;
"from" return FROM;
"where" return WHERE;
"like" return LIKE;
"between" return BETWEEN;
"not" return NOT;
"and" return AND;
"or" return OR;
"as" return AS;
[0-9]* yylval.num=atoi(yytext);return NUM;
"<>" return NE;
"<=" return LE;
">=" return GE;
[a-z][a-z0-9]* {
                realloc(yylval.str, strlen(yytext)*sizeof(char));
                strcpy(yylval.str, yytext);
	        return IDENTIFIER;
	       }


%%

int yywrap(void) { return 1; }
