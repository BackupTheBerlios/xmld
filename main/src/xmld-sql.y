%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_assign.h"
#include "xmld_func.h"
#include "xmld_cond.h"
#include "xmld_col.h"
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
#define YYPARSE_PARAM work
%}

%union {
 XMLDExpr *expr;
 XMLDCond *cond;
 XMLDList *list;
 char *qval;
 long num;
}

/* We may have a multi-threaded mtasker later */
%pure_parser

/* Keywords */
%token SELECT
%token FROM
%token WHERE
%token UPDATE
%token SET
%token DELETE
%token INSERT
%token INTO
%token VALUES
%token USE
%token AS

/* Type Definitions */
%type <expr> expr
%type <cond> cond
%type <list> expr_list cond_list

/* Other Tokens  */
%token <qval> QVAL IDENTIFIER
%token <num> NUM

/* Logical Operators */
%left AND OR
%nonassoc '=' '<' '>' NE LE GE LIKE BETWEEN NBETWEEN
%left NOT

/* Mathematical Operators */
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%%

query: SELECT expr_list FROM QVAL {
				   ((XMLDWork *) work)->req->type=0;
				   ((XMLDWork *) work)->req->file=$4;
				   ((XMLDWork *) work)->req->retr=$2;
				   YYACCEPT;
                                  }
       | SELECT expr_list FROM QVAL WHERE cond_list {
			     	                     ((XMLDWork *) work)->req->type=1;
				                     ((XMLDWork *) work)->req->file=$4;
				                     ((XMLDWork *) work)->req->retr=$2;
						     ((XMLDWork *) work)->req->where=$6;
						     YYACCEPT;
                                                    }
       | UPDATE QVAL SET cond_list {
				    ((XMLDWork *) work)->req->type=2;
				    ((XMLDWork *) work)->req->file=$2;
				    ((XMLDWork *) work)->req->vals=$4;
				    YYACCEPT;
                                   }
       | UPDATE QVAL SET cond_list WHERE cond_list {
				                    ((XMLDWork *) work)->req->type=3;
				                    ((XMLDWork *) work)->req->file=$2;
				                    ((XMLDWork *) work)->req->vals=$4;
						    ((XMLDWork *) work)->req->where=$6;
						    YYACCEPT;
                                                   }
       | DELETE FROM QVAL {
 		           ((XMLDWork *) work)->req->type=4;
			   ((XMLDWork *) work)->req->file=$3;
			   YYACCEPT;
                          }
       | DELETE FROM QVAL WHERE cond_list {
 		                           ((XMLDWork *) work)->req->type=5;
			                   ((XMLDWork *) work)->req->file=$3;
					   ((XMLDWork *) work)->req->where=$5;
					   YYACCEPT;
                                          }
       | DELETE '*' FROM QVAL {
 		               ((XMLDWork *) work)->req->type=4;
			       ((XMLDWork *) work)->req->file=$4;
			       YYACCEPT;
                              }
       | INSERT INTO QVAL '(' expr_list ')' VALUES '(' expr_list ')' {
                                                                      ((XMLDWork *) work)->req->type=6;
								      ((XMLDWork *) work)->req->file=$3;
								      ((XMLDWork *) work)->req->retr=$5;
								      ((XMLDWork *) work)->req->vals=$9;
								      YYACCEPT;
								     }								     
       | INSERT INTO QVAL VALUES '(' expr_list ')' {
                                                    ((XMLDWork *) work)->req->type=9;
					            ((XMLDWork *) work)->req->file=$3;
						    ((XMLDWork *) work)->req->vals=$6;
						    YYACCEPT;
                                                   }
       | INSERT INTO QVAL '(' expr_list ')' VALUES '(' expr_list ')' WHERE cond_list {
                                                                                      ((XMLDWork *) work)->req->type=7;
										      ((XMLDWork *) work)->req->file=$3;
										      ((XMLDWork *) work)->req->retr=$5;
										      ((XMLDWork *) work)->req->vals=$9;
										      ((XMLDWork *) work)->req->where=$12;
										      YYACCEPT;
										     }
       | INSERT INTO QVAL VALUES '(' expr_list ')' WHERE cond_list {
                                                                    ((XMLDWork *) work)->req->type=8;
						                    ((XMLDWork *) work)->req->file=$3;
								    ((XMLDWork *) work)->req->vals=$6;
								    ((XMLDWork *) work)->req->where=$9;
								    YYACCEPT;
                                                                   }
       | USE QVAL {
                   ((XMLDWork *) work)->req->type=10;
		   ((XMLDWork *) work)->req->file=$2;
		   YYACCEPT;
                  }
       | error {
                xmld_errno=XMLD_EPARSE;
                YYABORT; 
               }
;

cond_list: cond {
                 $$=XMLDCond_create_list();
		 XMLDCond *cond=XMLDCond_add_to_list($$);
		 XMLDCond_copy($1, cond);
		 XMLDCond_free($1);
                }
           | cond_list ':' cond {
                                 $$=$1;
		                 XMLDCond *cond=XMLDCond_add_to_list($$);
		                 XMLDCond_copy($1, cond);
		                 XMLDCond_free($1);
	                        }
	   | cond_list "," cond { /* for UPDATE support */
                                 $$=$1;
		                 XMLDCond *cond=XMLDCond_add_to_list($$);
		                 XMLDCond_copy($1, cond);
		                 XMLDCond_free($1);
	                        }
;

expr_list: expr {
                 $$=XMLDExpr_create_list();
		 XMLDExpr *expr=XMLDExpr_add_to_list($$);
		 XMLDExpr_copy($1, expr);
		 XMLDExpr_free($1);
                }
           | expr_list ',' expr {
                                 $$=$1;
		                 XMLDExpr *expr=XMLDExpr_add_to_list($$);
		                 XMLDExpr_copy($3, expr);
		                 XMLDExpr_free($3);
	                        }
;

cond: '(' cond ')' {
                    $$=$2;
		   }		   
      | expr '=' expr {
                       $$=XMLDCond_create();
		       $$->negate=0;
		       $$->type=0;
		       $$->left=$1;
		       $$->right=3;
		       $$->op=0;
                      }
      | expr '<' expr {
                       $$=XMLDCond_create();
		       $$->negate=0;
		       $$->type=0;
		       $$->left=$1;
		       $$->right=3;
		       $$->op=1;
                      }
      | expr '>' expr {
                       $$=XMLDCond_create();
		       $$->negate=0;
		       $$->type=0;
		       $$->left=$1;
		       $$->right=3;
		       $$->op=2;
                      }
      | expr NE expr {
                      $$=XMLDCond_create();
		      $$->negate=0;
		      $$->type=0;
		      $$->left=$1;
		      $$->right=3;
		      $$->op=3;
                     }
      | expr LE expr {
                      $$=XMLDCond_create();
		      $$->negate=0;
		      $$->type=0;
		      $$->left=$1;
		      $$->right=3;
		      $$->op=4;
		     } 
      | expr GE expr {
                      $$=XMLDCond_create();
		      $$->negate=0;
		      $$->type=0;
		      $$->left=$1;
		      $$->right=3;
		      $$->op=5;
		     }
      | expr LIKE expr {
                        $$=XMLDCond_create();
                        $$->negate=0;
                        $$->type=0;
                        $$->left=$1;
                        $$->right=$3;
			$$->op=6;
		       }
      | expr BETWEEN expr  {
                            $$=XMLDCond_create();
			    $$->negate=0;
			    $$->type=0;
			    $$->left=$1;
			    $$->right=$3;
			    $$->op=7;
                           }
      | expr NBETWEEN expr {
                            $$=XMLDCond_create();
			    $$->negate=0;
			    $$->type=0;
			    $$->left=$1;
			    $$->right=$3;
			    $$->op=8;
                           }
      | cond AND cond {
                       $$=XMLDCond_create();
		       $$->negate=0;
		       $$->type=1;
		       $$->cleft=$1;
		       $$->cright=$3;
		       $$->cop=0;
		      } 
      | cond OR cond {
                      $$=XMLDCond_create();
		      $$->negate=0;
		      $$->type=1;
		      $$->cleft=$1;
		      $$->cright=$3;
		      $$->cop=1;
                     }
      | NOT cond {
                  $$=$2;
		  $$->negate=~($$->negate);
                 }
      | '!' { /* void condition */
             $$=XMLDCond_create();
	     $$->type=2;
            }
;

expr: '(' expr ')' {
                    $$=$2;
                   }
      | NUM {
             $$=XMLDExpr_create();
	     $$->aggr=0;
	     $$->type=0;
	     $$->nval=$1;
            }
      | expr '+' expr {                       
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=0;
		      }		      
      | expr '-' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=1;
                      }
      | expr '*' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=2;                       
                      }
      | expr '/' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=3;
                      }
      | expr '^' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=4;		       
                      }
      | '-' expr %prec NEG {
                            $$=XMLDExpr_create();
			    $$->aggr=$2->aggr;
			    $$->type=1;
			    $$->right=$2;
			    $$->op=5;
                           }
      | expr AND expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=1;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=6;
                      }
      | IDENTIFIER {
                    $$=XMLDExpr_create();
		    $$->aggr=0;
		    $$->type=2;
		    $$->ident=$1;
                   }
      | IDENTIFIER '(' expr_list ')' {
                                      $$=XMLDExpr_create();
				      $$->type=3;
				      $$->func=XMLDFunc_search_list_by_name(func_list, $1);
				      $$->arg_list=$3;
				      $$->aggr=$$->func->aggr;
				      free($1);
                                     }
      | QVAL {
              $$=XMLDExpr_create();
	      $$->aggr=0;
	      $$->type=4;
	      $$->qval=$1;
             }
      | '*' {
             $$=XMLDExpr_create();
	     $$->aggr=0;
	     $$->type=5;
	     $$->wildcard=0;
            }
      | '@' {
             $$=XMLDExpr_create();
	     $$->aggr=0;
	     $$->type=5;
	     $$->wildcard=1;
            }
      | expr AS QVAL {
                      $$=$1;
		      $$->alias=$3;		      
                     }
;

%%

