/* FIXME: check how yylval would be cleaned after a realloc in the lexer
   that involves yylval */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "xmld_types.h"
int yylex(void);
int yyerror(char *);
void printout(char *);
%}

%union {
 char *str;
 struct XMLDCond *cond_t;
 char **str_arr;
 int num;
}

%token SELECT /* flex: "select" */
%token <str> IDENTIFIER    /* flex [A-Z][A-Z0-9]+ | "(tagname)" | "(text)" */
%token FROM   /* flex: "from"   */
%token WHERE  /* flex: "where"  */
%token <str> QUOTED_VAL /* flex: "\"[a-z]+\"" | "'[a-z]+'" */
%token <num> NUM /* flex: [0-9]* */
%type <cond_t> cond
%type <num> expr
%type <str_arr> retr
%token <cond_t> AND /* flex: "AND" */
%token <cond_t> OR /* flex: "OR" */
%token <cond_t> XOR /* flex: "XOR" */
%token NOT /* flex "NOT" */
%token <cond_t> LE /* flex: "<=" */
%token <cond_t> GE /* flex: ">=" */
%token <cond_t> NE /* flex: "<>" */
%token <cond_t> LIKE /* flex "like" */
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'
%%

query: /* empty */
       | SELECT retr FROM QUOTED_VAL
       | SELECT retr FROM QUOTED_VAL WHERE where
;

retr: '*' { $$=realloc(sizeof(char*)*(strlen($$)+1));*$$="*"; }
      | '@' { $$=realloc(sizeof(char*)*(strlen($$)+1));*$$="@";}
      | IDENTIFIER
      | retr ',' IDENTIFIER
;

where: cond
       | where ':' cond
;

cond: '(' cond ')' { $$ = $2; }
      | NOT cond   { $$ = $2;
                     $$->negate=($$->negate==1) ? 0 : 1; }
      | cond AND cond { $$=$2;
                        $$->left=$1;
			$$->right=$3;
			$$->connector=1;}
      | cond OR cond { $$=$2;
                       $$->left=$1;
		       $$->right=$3;
		       $$->connector=2;}
      | cond XOR cond {$$=$2;
                       $$->left=$1;
		       $$->right=$3;
		       $$->connector=3;}
      | 0             { $$=0; }
      | IDENTIFIER '=' QUOTED_VAL {
                             $$=$2;
			     $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			     strcpy($$->ident, $1);
			     $$->val=(char*)malloc(strlen($3)*sizeof(char));
			     strcpy($$->val, $3);
			     $$->op=0;
                            }
      | IDENTIFIER '=' expr {
                             $$=$2;
			     $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			     strcpy($$->ident,$1);
			     $$->val=0;
			     $$->nval=$3;
			     $$->op=0;
                             }
      | IDENTIFIER '<' expr {
                             $$=$2;
			     $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			     strcpy($$->ident, $1);
			     $$->val=0;
			     $$->nval=$3;
			     $$->op=1;
                            }
      | IDENTIFIER '>' expr {
                             $$=$2;
			     $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			     strcpy($$->ident, $1);
			     $$->val=0;
			     $$->nval=$3;
			     $$->op=2;
                            }
      | IDENTIFIER LE expr {
                           $$=$2;
			   $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			   strcpy($$->ident, $1);
			   $$->val=0;
			   $$->nval=$3;
			   $$->op=3;
                          }
      | IDENTIFIER GE expr {
                            $$=$2;
			    $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			    strcpy($$->ident, $1);
			    $$->val=0;
			    $$->nval=$3;
			    $$->op=4;
                           }
      | IDENTIFIER LIKE QUOTED_VAL  {
                                     $$=$2;
			             $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			             strcpy($$->ident, $1);
			             $$->val=(char*)malloc(strlen($3)*sizeof(char));
			             strcpy($$->val, $3);
			             $$->op=5;
                                    }
      | IDENTIFIER NE expr {
                            $$=$2;
			    $$->ident=(char*)malloc(strlen($1)*sizeof(char));
			    strcpy($$->ident,$1);
			    $$->val=0;
			    $$->nval=$3;
			    $$->op=6;
                           }
      | IDENTIFIER NE QUOTED_VAL {
                                  $$=$2;
				  $$->ident=(char*)malloc(strlen($1)*sizeof(char));
				  strcpy($$->ident,$1);
				  $$->val=(char*)malloc(strlen($3)*sizeof(char));
				  strcpy($$->val, $3);
				  $$->op=6;
                                 }
;

expr: NUM 
      | expr '+' expr {$$ = $1 + $3; }
      | expr '-' expr {$$ = $1 - $3; }
      | expr '*' expr {$$ = $1 * $3; }
      | expr '/' expr {$$ = $1 / $3; }
      | expr '^' expr {$$ = pow($1,$3); }
      | '-' expr %prec NEG {$$ = -$2}
      | '(' expr ')' { $$ = $2; }
;
%%

void printout(char *s) {
 printf("%s\n",s);
}
int yyerror(char *s) {
 printf("%s\n",s);
 return 0;
}
