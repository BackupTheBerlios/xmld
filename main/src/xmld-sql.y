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
#include "func_list.h"
#define YYPARSE_PARAM fd
#define YYLEX_PARAM fd
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
%type <list> expr_list cond_list assign_list

/* Other Tokens  */
%token <qval> QVAL IDENTIFIER
%token <num> NUM

/* Conditional Operators */
%left AND OR
%nonassoc '=' '<' '>' NE LE GE LIKE BETWEEN NBETWEEN
%left NOT

/* Mathematical Operators */
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

%%

query: SELECT expr_list FROM QVAL
       | SELECT expr_list FROM QVAL WHERE cond_list
       | UPDATE QVAL SET assign_list
       | UPDATE QVAL SET assign_list WHERE cond_list
       | DELETE FROM QVAL
       | DELETE FROM QVAL WHERE cond_list
       | DELETE '*' FROM QVAL
       | INSERT INTO QVAL '(' expr_list ')' VALUES '(' expr_list ')'
       | INSERT INTO QVAL VALUES '(' expr_list ')'
       | INSERT INTO QVAL '(' expr_list ')' VALUES '(' expr_list ')' WHERE cond_list
       | INSERT INTO QVAL VALUES '(' expr_list ')' WHERE cond_list
       | USE QVAL
       | error { YYABORT; }
;

assign_list: assign
             | assign_list ',' assign
;

cond_list: cond
           | cond_list ':' cond
;

expr_list: expr
           | expr_list ',' expr
;

assign: IDENTIFIER '=' expr
;

cond: '(' cond ')'
      | expr '=' expr
      | expr '<' expr
      | expr '>' expr
      | expr NE expr
      | expr LE expr
      | expr GE expr
      | expr LIKE expr
      | expr BETWEEN expr
      | expr NBETWEEN expr
      | cond AND cond
      | cond OR cond
      | NOT cond
;

expr: '(' expr ')'
      | NUM
      | expr '+' expr
      | expr '-' expr
      | expr '*' expr
      | expr '/' expr
      | expr '^' expr
      | '-' expr %prec NEG
      | expr AND expr
      | IDENTIFIER
      | IDENTIFIER '(' expr_list ')'
      | QVAL
      | '*'
      | '@'
      | expr AS QVAL
;

%%

