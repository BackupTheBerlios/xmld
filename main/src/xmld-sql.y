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
#define YYPARSE_PARAM work
#include "qp.h"
#include "xmld-sql.h"
%}

%union {
 XMLDExpr *expr;
 XMLDCond *cond;
 XMLDList *list;
 char *qval;
 int num;
 float fnum;
}

/* We may have a multi-threaded mtasker later */
%pure_parser
%expect 10

/* Keywords */
%token SELECT
%token DISCONNECT
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
%token <fnum> FNUM

/* Level Handling Operators */
%left ':'
%left '~'

/* Composition Operator */
%left ','

/* Logical Operators */
%left AND OR
%nonassoc '=' '<' '>' NE LE GE LIKE BETWEEN NBETWEEN
%left NOT

/* Mathematical Operators */
%left '+' '-'
%left '*' '/'
%left NEG
%right '^'

/* Relation operator */
%nonassoc '.'

%%

query: SELECT expr_list FROM expr {
				   ((XMLDWork *) work)->req->type=XMLD_SQL_SELECT;
                                   ((XMLDWork *) work)->files=XMLDFileList_create();
                                   if ($4->type == XMLD_QVAL) {
                                    XMLDFileList_add(((XMLDWork *) work)->files, $4->qval);
                                   }
                                   else if ($4->type == XMLD_LIST) {
                                    XMLDList_reset($4->exprs);
                                    while (XMLDList_next($4->exprs)) {
                                     XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($4->exprs))->qval);
                                    }
                                   }
	                           XMLDExpr_free($4);
				   ((XMLDWork *) work)->req->retr=$2;
				   YYACCEPT;
                                  }
       | SELECT expr_list FROM expr WHERE cond_list {
			     	                     ((XMLDWork *) work)->req->type=XMLD_SQL_SELECT_WHERE;
                                                     ((XMLDWork *) work)->files=XMLDFileList_create();
                                                     if ($4->type == XMLD_QVAL) {
                                                      XMLDFileList_add(((XMLDWork *) work)->files, $4->qval);
                                                     }
                                                     else if ($4->type == XMLD_LIST) {
                                                      XMLDList_reset($4->exprs);
                                                      while (XMLDList_next($4->exprs)) {
	                                               XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($4->exprs))->qval);
                                                      }
			                             }
			                             XMLDExpr_free($4);
				                     ((XMLDWork *) work)->req->retr=$2;
						     ((XMLDWork *) work)->req->where=$6;
						     YYACCEPT;
                                                    }
       | UPDATE expr SET cond_list {
				    ((XMLDWork *) work)->req->type=XMLD_SQL_UPDATE;
                                    ((XMLDWork *) work)->files=XMLDFileList_create();
                                    if ($2->type == XMLD_QVAL) {
                                     XMLDFileList_add(((XMLDWork *) work)->files, $2->qval);
                                    }
                                    else if ($2->type == XMLD_LIST) {
                                     XMLDList_reset($2->exprs);
                                     while (XMLDList_next($2->exprs)) {
	                              XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($2->exprs))->qval);
                                     }
			            }
			            XMLDExpr_free($2);
				    ((XMLDWork *) work)->req->vals=$4;
				    YYACCEPT;
                                   }
       | UPDATE expr SET cond_list WHERE cond_list {
				                    ((XMLDWork *) work)->req->type=XMLD_SQL_UPDATE_WHERE;
	                                            ((XMLDWork *) work)->files=XMLDFileList_create();
	                                            if ($2->type == XMLD_QVAL) {
                                                     XMLDFileList_add(((XMLDWork *) work)->files, $2->qval);
	                                            }
	                                            else if ($2->type == XMLD_LIST) {
	                                             XMLDList_reset($2->exprs);
	                                             while (XMLDList_next($2->exprs)) {
			                              XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($2->exprs))->qval);
			                             }
			                            }
			                            XMLDExpr_free($2);
				                    ((XMLDWork *) work)->req->vals=$4;
						    ((XMLDWork *) work)->req->where=$6;
						    YYACCEPT;
                                                   }
       | DELETE FROM expr {
 		           ((XMLDWork *) work)->req->type=XMLD_SQL_DELETE;
	                   ((XMLDWork *) work)->files=XMLDFileList_create();
	                   if ($3->type == XMLD_QVAL) {
                            XMLDFileList_add(((XMLDWork *) work)->files, $3->qval);
	                   }
	                   else if ($3->type == XMLD_LIST) {
	                    XMLDList_reset($3->exprs);
	                    while (XMLDList_next($3->exprs)) {
			     XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($3->exprs))->qval);
			    }
			   }
			   XMLDExpr_free($3);
			   YYACCEPT;
                          }
       | DELETE FROM expr WHERE cond_list {
 		                           ((XMLDWork *) work)->req->type=XMLD_SQL_DELETE_WHERE;
			                   ((XMLDWork *) work)->files=XMLDFileList_create();
			                   if ($3->type == XMLD_QVAL) {
                                            XMLDFileList_add(((XMLDWork *) work)->files, $3->qval);
			                   }
			                   else if ($3->type == XMLD_LIST) {
			                    XMLDList_reset($3->exprs);
				            while (XMLDList_next($3->exprs)) {
				             XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($3->exprs))->qval);
				            }
			                   }
			                   XMLDExpr_free($3);
					   ((XMLDWork *) work)->req->where=$5;
					   YYACCEPT;
                                          }
       | DELETE '*' FROM expr {
 		               ((XMLDWork *) work)->req->type=XMLD_SQL_DELETE;
			       ((XMLDWork *) work)->files=XMLDFileList_create();
			       if ($4->type == XMLD_QVAL) {
                                XMLDFileList_add(((XMLDWork *) work)->files, $4->qval);				
			       }
			       else if ($4->type == XMLD_LIST) {
			        XMLDList_reset($4->exprs);
				while (XMLDList_next($4->exprs)) {
				 XMLDFileList_add(((XMLDWork *) work)->files, ((XMLDExpr *) XMLDList_curr($4->exprs))->qval);
				}
			       }
			       XMLDExpr_free($4);
			       YYACCEPT;
                              }
       | INSERT INTO expr '(' expr_list ')' VALUES '(' expr_list ')' {
                                                                      ((XMLDWork *) work)->req->type=XMLD_SQL_INSERT_COL;
								      ((XMLDWork *) work)->files=XMLDFileList_create();
		                                                      char *name=NULL;
						                      if ($3->type == XMLD_QVAL) {
						                       name=$3->qval;
						                      }
						                      else if ($3->type == XMLD_LIST) {
						                       name=((XMLDExpr *) XMLDList_first($3->exprs))->qval;
						                      }
						                      XMLDFileList_add(((XMLDWork *) work)->files, name);
								      XMLDExpr_free($3);
								      ((XMLDWork *) work)->req->retr=$5;
								      ((XMLDWork *) work)->req->vals=$9;
								      YYACCEPT;
								     }								     
       | INSERT INTO expr VALUES '(' expr_list ')' {
                                                    ((XMLDWork *) work)->req->type=XMLD_SQL_INSERT;
					            ((XMLDWork *) work)->files=XMLDFileList_create();
		                                    char *name=NULL;
						    if ($3->type == XMLD_QVAL) {
						     name=$3->qval;
						    }
						    else if ($3->type == XMLD_LIST) {
						     name=((XMLDExpr *) XMLDList_first($3->exprs))->qval;
						    }
						    XMLDFileList_add(((XMLDWork *) work)->files, name);
						    XMLDExpr_free($3);
					            ((XMLDWork *) work)->req->vals=$6;
						    YYACCEPT;
                                                   }
       | INSERT INTO expr '(' expr_list ')' VALUES '(' expr_list ')' WHERE cond_list {
                                                                                      ((XMLDWork *) work)->req->type=XMLD_SQL_INSERT_COL_WHERE;
										      ((XMLDWork *) work)->files=XMLDFileList_create();
								                      char *name=NULL;
								                      if ($3->type == XMLD_QVAL) {
								                       name=$3->qval;
								                      }
								                      else if ($3->type == XMLD_LIST) {
								                       name=((XMLDExpr *) XMLDList_first($3->exprs))->qval;
								                      }
								                      XMLDFileList_add(((XMLDWork *) work)->files, name);
										      XMLDExpr_free($3);
										      ((XMLDWork *) work)->req->retr=$5;
										      ((XMLDWork *) work)->req->vals=$9;
										      ((XMLDWork *) work)->req->where=$12;
										      YYACCEPT;
										     }
       | INSERT INTO expr VALUES '(' expr_list ')' WHERE cond_list {
                                                                    ((XMLDWork *) work)->req->type=XMLD_SQL_INSERT_WHERE;
						                    ((XMLDWork *) work)->files=XMLDFileList_create();
								    char *name=NULL;
								    if ($3->type == XMLD_QVAL) {
								     name=$3->qval;
								    }
								    else if ($3->type == XMLD_LIST) {
								     name=((XMLDExpr *) XMLDList_first($3->exprs))->qval;
								    }
								    XMLDFileList_add(((XMLDWork *) work)->files, name);
								    XMLDExpr_free($3);
								    ((XMLDWork *) work)->req->vals=$6;
								    ((XMLDWork *) work)->req->where=$9;
								    YYACCEPT;
                                                                   }
       | USE expr_list {
                        ((XMLDWork *) work)->req->type=XMLD_SQL_USE;
		        ((XMLDWork *) work)->req->retr=$2;
		        YYACCEPT;
                       }
       | error {
                xmld_errno=XMLD_EPARSE;
                YYABORT;
               }
;

				     
cond_list: cond {
                 $$=XMLDCondList_create();
		 XMLDCond *cond=XMLDCondList_add($$);
		 XMLDCond_copy($1, cond);
		 free($1);
                }
	   | cond_list ':' cond {
                                 $$=$1;
		                 XMLDCond *cond=XMLDCondList_add($$);
		                 XMLDCond_copy($3, cond);
		                 free($3);
	                        }
;

expr_list: expr {
                 $$=XMLDExprList_create();
		 XMLDExpr *expr=XMLDExprList_add($$);
		 XMLDExpr_copy($1, expr);
		 free($1);
                }
           | expr_list ':' expr {
                                 $$=$1;
		                 XMLDExpr *expr=XMLDExprList_add($$);
		                 XMLDExpr_copy($3, expr);
		                 free($3);
	                        }
	   /* These two rules are to handle void expression lists */
	   | expr_list ':' cond {                         
                                 $$=$1;
		                 free($3);
		                 XMLDExpr *expr=XMLDExprList_add($$);
		                 expr->type = XMLD_VOID_LIST;
				 expr->aggr = XMLD_FALSE;
	                        }
	   | cond_list ':' expr {
	                    $$=XMLDExprList_create();
			    XMLDList_free($1);
		            XMLDExpr *expr=XMLDExprList_add($$);
			    expr->type = XMLD_VOID_LIST;
			    expr->aggr = XMLD_FALSE;
			    expr=XMLDExprList_add($$);
			    XMLDExpr_copy($3, expr);
			    free($3);
			   }
;

cond: '(' cond ')' {
                    $$=$2;
		   }		   
      | expr '=' expr {
                       $$=XMLDCond_create();
		       $$->negate=XMLD_FALSE;
		       $$->type=XMLD_CONDITION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_COND_OP_EQUAL;
                      }
      | expr '<' expr {
                       $$=XMLDCond_create();
		       $$->negate=XMLD_FALSE;
		       $$->type=XMLD_CONDITION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_COND_OP_L;
                      }
      | expr '>' expr {
                       $$=XMLDCond_create();
		       $$->negate=XMLD_FALSE;
		       $$->type=XMLD_CONDITION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_COND_OP_G;
                      }
      | expr NE expr {
                      $$=XMLDCond_create();
		      $$->negate=XMLD_FALSE;
		      $$->type=XMLD_CONDITION;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=XMLD_COND_OP_NE;
                     }
      | expr LE expr {
                      $$=XMLDCond_create();
		      $$->negate=XMLD_FALSE;
		      $$->type=XMLD_CONDITION;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=XMLD_COND_OP_LE;
		     } 
      | expr GE expr {
                      $$=XMLDCond_create();
		      $$->negate=XMLD_FALSE;
		      $$->type=XMLD_CONDITION;
		      $$->left=$1;
		      $$->right=$3;
		      $$->op=XMLD_COND_OP_GE;
		     }
      | expr LIKE expr {
                        $$=XMLDCond_create();
                        $$->negate=XMLD_FALSE;
                        $$->type=XMLD_CONDITION;
                        $$->left=$1;
                        $$->right=$3;
			$$->op=XMLD_COND_OP_LIKE;
		       }
      | expr BETWEEN expr  {
                            $$=XMLDCond_create();
			    $$->negate=XMLD_FALSE;
			    $$->type=XMLD_CONDITION;
			    $$->left=$1;
			    $$->right=$3;
			    $$->op=XMLD_COND_OP_BET;
                           }
      | expr NBETWEEN expr {
                            $$=XMLDCond_create();
			    $$->negate=XMLD_FALSE;
			    $$->type=XMLD_CONDITION;
			    $$->left=$1;
			    $$->right=$3;
			    $$->op=XMLD_COND_OP_NBET;
                           }
      | cond AND cond {
                       $$=XMLDCond_create();
		       $$->negate=XMLD_FALSE;
		       $$->type=XMLD_CONDITION_GRP;
		       $$->cleft=$1;
		       $$->cright=$3;
		       $$->cop=XMLD_COND_GRP_AND;
		      } 
      | cond OR cond {
                      $$=XMLDCond_create();
		      $$->negate=XMLD_FALSE;
		      $$->type=XMLD_CONDITION_GRP;
		      $$->cleft=$1;
		      $$->cright=$3;
		      $$->cop=XMLD_COND_GRP_OR;
                     }
      | NOT cond {
                  $$=$2;
		  $$->negate=!($$->negate);
                 }
      | '!' { /* void condition */
             $$=XMLDCond_create();
	     $$->type=XMLD_CONDITION_VOID;
            }
      | cond ',' cond {
                       XMLDCond *cond;
		       if ($1->type != XMLD_CONDITION_LIST) {
		        $$=XMLDCond_create();
			$$->type = XMLD_CONDITION_LIST;
			$$->negate=XMLD_FALSE;
			$$->conds = XMLDCondList_create();
			cond=XMLDCondList_add($$->conds);
			XMLDCond_copy($1, cond);
			free($1);
			cond=XMLDCondList_add($$->conds);
			XMLDCond_copy($3, cond);
			free($3);
		       }
		       else {
		        $$=$1;
			$$->negate=XMLD_FALSE;
			cond=XMLDCondList_add($$->conds);
			XMLDCond_copy($3, cond);
			free($3);
		       }
		      }
     | '~' cond {
                 $$=$2;
		 $2->cross_level=XMLD_TRUE;
                }
;

expr: '(' expr ')' {
                    $$=$2;
                   }
      | NUM {
             $$=XMLDExpr_create();
	     $$->aggr=XMLD_FALSE;
	     $$->type=XMLD_INTEGER;
	     $$->nval=$1;
            }
      | FNUM {
              $$=XMLDExpr_create();
	      $$->aggr=XMLD_FALSE;
	      $$->type=XMLD_FLOAT;
	      $$->fnval=$1;
             }
      | expr '+' expr {                       
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_ADD;
		      }		      
      | expr '-' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_BNEG;
                      }
      | expr '*' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_MULTIP;
                      }
      | expr '/' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_DIV;
                      }
      | expr '^' expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_EXPO;
                      }
      | '-' expr %prec NEG {
                            $$=XMLDExpr_create();
			    $$->aggr=$2->aggr;
			    $$->type=XMLD_OPERATION;
			    $$->right=$2;
			    $$->op=XMLD_OP_UNEG;
                           }
      | expr AND expr {
                       $$=XMLDExpr_create();
		       $$->aggr=($1->aggr || $3->aggr);
		       $$->type=XMLD_OPERATION;
		       $$->left=$1;
		       $$->right=$3;
		       $$->op=XMLD_OP_AND;
                      }
      | IDENTIFIER {
                    $$=XMLDExpr_create();
		    $$->aggr=XMLD_FALSE;
		    $$->type=XMLD_IDENTIFIER;
		    $$->ident=$1;
                   }
      | SPECIAL_IDENTIFIER {
                            $$=XMLDExpr_create();
			    $$->aggr=XMLD_FALSE;
			    $$->type=XMLD_SPECIAL_IDENTIFIER;
			    if (strcmp($1, "[text]") == 0) {
			     $$->sident=XMLD_SIDENT_TEXT;
			    }
			    else {
			     $$->sident=XMLD_SIDENT_TAGNAME;
			    }
                           }
      | IDENTIFIER '(' expr_list ')' {
                                      $$=XMLDExpr_create();
				      $$->type=XMLD_FUNCTION;
				      $$->func=XMLDFuncList_search_by_name(func_list, $1);
				      $$->arg_list=$3;
				      $$->aggr=$$->func->aggr;
				      free($1);
                                     }
      | IDENTIFIER '(' ')' {
                            $$=XMLDExpr_create();
		            $$->type=XMLD_FUNCTION;
			    $$->func=XMLDFuncList_search_by_name(func_list, $1);
			    $$->aggr=$$->func->aggr;
			    free($1);
                           }
      | QVAL {
              $$=XMLDExpr_create();
	      $$->aggr=XMLD_FALSE;
	      $$->type=XMLD_QVAL;
	      $$->qval=$1;
             }
      | '*' {
             $$=XMLDExpr_create();
	     $$->aggr=XMLD_FALSE;
	     $$->type=XMLD_WILDCARD;
	     $$->wildcard=XMLD_WILDCARD_ALL;
            }
      | '@' {
             $$=XMLDExpr_create();
	     $$->aggr=XMLD_FALSE;
	     $$->type=XMLD_WILDCARD;
	     $$->wildcard=XMLD_WILDCARD_ATTS;
            }
      | expr AS QVAL {
                      $$=$1;
		      $$->alias=$3;		      
                     }
      | expr ',' expr {
                       XMLDExpr *expr;
		       if ($1->type != XMLD_LIST) {
		        $$=XMLDExpr_create();
			$$->type = XMLD_LIST;
			$$->aggr=$1->aggr || $3->aggr;
			$$->exprs = XMLDExprList_create();
			expr=XMLDExprList_add($$->exprs);
			XMLDExpr_copy($1, expr);
			free($1);
			expr=XMLDExprList_add($$->exprs);
			XMLDExpr_copy($3, expr);
			free($3);
		       }
		       else {
		        $$=$1;
			$$->aggr = $$->aggr || $3->aggr;
			expr=XMLDExprList_add($$->exprs);
			XMLDExpr_copy($3, expr);
			free($3);
		       }
                      }
      | '~' expr {
                  $$=$2;
		  $$->cross_level=XMLD_TRUE;
                 }
     /* Support for column -> file association */
     | expr '.' expr {
		      $$=$3;
		      $$->file=XMLDFile_create($1->qval);
		      XMLDExpr_free($1);
                     }
;

%%

void yyerror(char *s) {
}
