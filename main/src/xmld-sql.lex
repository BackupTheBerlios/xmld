%{
#include <stdlib.h>
#include <string.h>
#include "xmld-sql.tab.h"
#define YY_DECL int yylex(YYSTYPE *lvalp);
%}

%%

select return SELECT;
from return FROM;
where return WHERE;
update return UPDATE;
set return SET;
delete return DELETE;
insert return INSERT;
into return INTO;
values return VALUES;
use return USE;
as return AS;
and return AND;
or return OR;
"<>" return NE;
"<=" return LE;
">=" return GE;
like return LIKE;
between return BETWEEN;
"not between" return NBETWEEN;
\"[A-Z0-9]+\" {
               lvalp->qval=(char *) malloc((strlen(yytext)+1)*sizeof(char));
	       strcpy(lvalp->qval, yytext);
               return QVAL;
	      }
[A-Z][A-Z0-9]* {
                   lvalp->qval=(char *) malloc((strlen(yytext)+1)*sizeof(char));
		   strcpy(lvalp->qval, yytext);
		   return IDENTIFIER;
                  }
[0-9]+ {
        lvalp->num=atol(yytext);
	return NUM;
       }
. return yytext[0];

%%

