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

#define YYPARSE_PARAM cfg_tree
%}

%union {
 int integer;
 char *string;
 XMLDCfgValue *value;
 XMLDCfgDirective *directive;
 XMLDCfgSection *section;
 XMLDAssoc *list;
}

/* Thread-safe */
%pure_parser

/* Types and tokens */
%type <list> value_list
%type <section> cfg_tree section
%type <directive> directive
%type <value> value
%token <string> IDENTIFIER STRING
%token <integer> INTEGER

%%

configuration: cfg_tree
	     {
	      ((XMLDCfgSection *) cfg_tree) = $1;
	      YYACCEPT;
	     }
;

cfg_tree: directive
	  {
	   $$=XMLDCfgSection_create();
           $$->directives = XMLDAssoc_create();
	   XMLDAssoc_add($$->directives, $1->name, $1);
	   free($1->name);
	  }
	| section
	  {
	   $$=XMLDCfgSection_create();
	   $$->sections = XMLDAssoc_create();
	   XMLDAssoc_add($$->sections, $1->name, $1);
	   free($1->name);
	  }
	| cfg_tree '\n' directive
	  {
	   $$=$1;
	   XMLDAssoc_add($$->directives, $3->name, $3);
	   free($3->name);
	  }
	| cfg_tree '\n' section 
	  {
	   $$=$1;
	   XMLDAssoc_add($$->sections, $3->name, $3)
	   free($3->name);
	  }
;

section: '<' IDENTIFIER '>' '\n' cfg_tree '\n' '<' '/' IDENTIFIER '>'
         {
	  $$=XMLDCfgSection_create();
	  $$->name = $2;
	  $$->directives = $5;
	 }
;

directive: IDENTIFIER ' ' value_list
	   {
	    $$=XMLDCfgDirective_create();
	    $$->name = $1;
	    $$->values = $3;
	   }
;

value_list: value
	         {
		  $$=XMLDAssoc_create();
	          XMLDAssoc_add($$, NULL, $1);
		 }
	  | value_list ' ' value
	         {
		  $$=$1;
		  XMLDAssoc_add($$, NULL, $3);
	         }
;
	  
value: STRING 
              {
               $$=XMLDCfgValue_create();
	       $$->type = XMLD_CFG_STRING;
	       $$->value = (void *) $1;
              }
     | INTEGER
              {
               $$=XMLDCfgValue_create();
	       $$->type = XMLD_CFG_INTEGER;
	       $$->value = (void *) $1;
	      }
;

%%

int yyerror(char *s) {
 return 0;
}
