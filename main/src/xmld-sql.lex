%{
#include <stdlib.h>
#include "xmld_types.h"
#include "xmld-sql.tab.h"
int yyparse(void);
%}

%%

"select" return SELECT;
"from" return FROM;
"where" return WHERE;
"and" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return AND;
"or" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return OR;
"XOR" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return XOR;
"NOT" return NOT;
"=" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return  '=';
"<=" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return LE;
">=" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return GE;
"<>" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return NE;
"<" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return  '<';
">" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return  '>';
":" return ':'; /* FIXME: return a bigger cond ? */
"like" yylval.cond_t=(struct XMLDCond *)malloc(sizeof(struct XMLDCond));return LIKE;
/* FIXME: check if QUOTED_VAL should be up */
"\"[a-z0-9]+\""|"'[a-z0-9]+'" yylval.str=(char*)realloc(yylval.str, strlen(yytext)*sizeof(char));strcpy(yylval.str,yytext);return QUOTED_VAL; 
[a-z][a-z0-9]+|"(tagname)"|"(text)" yylval.str=(char*)realloc(yylval.str, strlen(yytext)*sizeof(char));strcpy(yylval.str, yytext);return IDENTIFIER;
[0-9]* yylval.num=atoi(yytext); return NUM;

%%

int yywrap(void) {
 return 1;
}

int main(void) {
 yyparse();
 return 0;
}

