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
#include <string.h>
#include "errors.h"
#include "xmld_types.h"
#include "xmld_mempool.h"
#define  YYERROR_VERBOSE
int yyerror(char *);
int yylex(void);
int num_expr;
int num_cond;
struct XMLDRequest *request;
%}

%pure_parser

%union {
 int num;
 char *str;
 struct expr *expression;
 struct expr **expression_list;
 struct cond *condition;
 struct cond **condition_list;
}

%token SELECT
%token FROM
%token <str> QUOTED_VAL
%token WHERE
%type <num> query
%type <condition> cond
%type <expression> expr
%type <condition_list> cond_list
%type <expression_list> expr_list
%token LIKE
%token BETWEEN
%token <num> NUM
%token <str> IDENTIFIER
%token AS
%right NOT
%left AND OR
%nonassoc NE
%nonassoc LE
%nonassoc GE
%nonassoc '>'
%nonassoc '<'
%nonassoc '='
%left '-' '+'
%left '*' '/'
%left NEG
%right '^'

%%


query: /* empty */
       | SELECT expr_list FROM QUOTED_VAL {
                                           $$=1;
					   request=(struct XMLDRequest *) malloc(sizeof(XMLDRequest));
					   request->file=(char *) malloc(strlen($4)*sizeof(char));
					   strcpy(request->file, $4);
					   request->type=0;
					   request->retr=$2;
					   YYACCEPT;
                                          }
       | SELECT expr_list FROM QUOTED_VAL WHERE cond_list {
                                                           $$=1;
							   request=(struct XMLDRequest *) malloc(sizeof(XMLDRequest));
							   request->file=(char*) malloc(strlen($4)*sizeof(char));
							   strcpy(request->file, $4);
							   request->type=1;
							   request->retr=$2;
							   request->where=$6;
							   YYACCEPT;
                                                          }
;

cond_list: cond { 
                 $$=(struct cond **) malloc(2*sizeof(struct cond *));
                 $$[0]=$1;
		 $$[1]=(struct cond *) 0;
		 num_cond=2;
		}
           | cond_list ':' cond {
	                         $$=$1;
				 num_cond++;
				 $$=realloc($$, num_cond*sizeof(struct cond*));
				 $$[num_cond-2]=$3;
				 $$[num_cond-1]=(struct cond *) 0;
	                        }
;

cond: expr '=' expr {
                     $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		     $$->type=0;
		     $$->negate=0;
		     $$->left=$1;
		     $$->right=$3;
		     $$->op=0;
                    }
      | expr '<' expr {
                       $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		       $$->type=0;
		       $$->negate=0;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=1;
                      }  
      | expr '>' expr {
                       $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		       $$->type=0;
		       $$->negate=0;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=2;
                      } 
      | expr NE expr {
                      $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		      $$->type=0;
		      $$->negate=0;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=3;
                     }
      | expr LE expr {
                      $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		      $$->type=0;
		      $$->negate=0;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=4;
                     }
      | expr GE expr {
                      $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		      $$->type=0;
		      $$->negate=0;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=5;
                     }
      | expr LIKE expr {
                        $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
			$$->type=0;
			$$->negate=0;
			$$->left=$1;
			$$->right=$3;
			$$->op=6;
                       }
      /* FIXME: how should BETWEEN and NOT BETWEEN be manipulated ?
       * Add an op to expr (6) = AND ?? */
      | expr BETWEEN expr AND expr
      | expr NOT BETWEEN expr AND expr
      | NOT cond {
                  $$=$2;
		  $$->negate=(!$$->negate);
		 }
      | cond AND cond {
                       $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		       $$->type=1;
		       $$->negate=0;
		       $$->cleft=$1;
		       $$->cright=$3;
		       $$->coop=0;
                      }
      | cond OR cond {
                      $$=(struct cond *) XMLDMemPool_get_segment(cond_pool);
		      $$->type=1;
		      $$->negate=0;
		      $$->cleft=$1;
		      $$->cright=$3;
		      $$->coop=1;
                     }
      | '(' cond ')' {
                      $$=$2;
                     }
;

expr: NUM {
           $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
	   $$->type=0;
	   $$->aggr=0;
	   $$->nval=$1;
          }
      | QUOTED_VAL {
                    $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		    $$->type=4;
		    $$->aggr=0;
		    $$->qval=(char *) malloc(strlen($1)*sizeof(char));
		    strcpy($$->qval, $1);
                   }
      | IDENTIFIER {
                    $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		    $$->type=2;
		    $$->aggr=0;
		    $$->ident=(char *) malloc(strlen($1)*sizeof(char));
		    strcpy($$->ident, $1);
                   }
      | IDENTIFIER '(' expr_list ')' {
                                      $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
				      $$->type=3;
				      $$->func=0/* get the XMLDFunc pointer 
				                  from the sumbol table*/;
				      $$->aggr=$$->func->aggr;
				      $$->arg_list=$3;				      
                                     }
      | expr '+' expr {
                       $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		       $$->type=1;
		       $$->aggr=($1->aggr && $3->aggr);
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=0;
                      }
      | expr '-' expr {
                       $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		       $$->type=1;
		       $$->aggr=($1->aggr && $3->aggr);
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=1;
                      }
      | expr '*' expr {
                       $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		       $$->type=1;
		       $$->aggr=($1->aggr && $3->aggr);
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=2;
                      }
      | expr '/' expr {
                       $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		       $$->type=1;
		       $$->aggr=($1->aggr && $3->aggr);
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=3;
                      }
      | expr '^' expr {
                       $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
		       $$->type=1;
		       $$->aggr=($1->aggr && $3->aggr);
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=4;
                      }
      | '-' expr %prec NEG {
                            $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
			    $$->type=1;
			    $$->aggr=$2->aggr;
			    $$->left=(struct expr *)0;
			    $$->right=$2;
			    $$->op=5;
			   }
      | '(' expr ')' { $$=$2; }
      | expr AS QUOTED_VAL /* alias */
                           {
			    $$=$1;
			    /* FIXME: check if it's neccessary to strcpy */
			    $$->alias=(char *) malloc(strlen($3)*sizeof(char));
			    strcpy($$->alias, $3);
			   }
      | '*' {
             $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
	     $$->type=5;
	     $$->aggr=1;
	     $$->wildcard=0;
            }
      | '@' { 
             $$=(struct expr *) XMLDMemPool_get_segment(expr_pool);
             $$->type=5;
	     $$->aggr=1;
	     $$->wildcard=1;
	    }
;

expr_list: expr { 
                 $$=(struct expr **) malloc(2*sizeof(struct expr *));
                 $$[0]=$1;
		 $$[1]=(struct expr *) 0;
		 num_expr=2;
		}
           | expr_list ',' expr {
	                         $$=$1;
				 num_expr++;
				 $$=realloc($$, num_expr*sizeof(struct expr*));
				 $$[num_expr-2]=$3;
				 $$[num_expr-1]=(struct expr *) 0;
	                        }
;
%%

int yyerror(char *s) {
 ERROR_RESPONSE(s);
 return 0;
}
