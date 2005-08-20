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
 CfgValue *value;
 CfgDirective *directive;
 CfgSection *section;
 Assoc *list;
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
	      ((CfgSection *) cfg_tree) = $1;
	      YYACCEPT;
	     }
;

cfg_tree: directive
	  {
	   $$=CfgSection_create();
           $$->directives = Assoc_create();
	   Assoc_add($$->directives, $1->name, $1);
	   free($1->name);
	  }
	| section
	  {
	   $$=CfgSection_create();
	   $$->sections = Assoc_create();
	   Assoc_add($$->sections, $1->name, $1);
	   free($1->name);
	  }
	| cfg_tree '\n' directive
	  {
	   $$=$1;
	   Assoc_add($$->directives, $3->name, $3);
	   free($3->name);
	  }
	| cfg_tree '\n' section 
	  {
	   $$=$1;
	   Assoc_add($$->sections, $3->name, $3)
	   free($3->name);
	  }
;

section: '<' IDENTIFIER '>' '\n' cfg_tree '\n' '<' '/' IDENTIFIER '>'
         {
	  $$=CfgSection_create();
	  $$->name = $2;
	  $$->directives = $5;
	 }
;

directive: IDENTIFIER ' ' value_list
	   {
	    $$=CfgDirective_create();
	    $$->name = $1;
	    $$->values = $3;
	   }
;

value_list: value
	         {
		  $$=Assoc_create();
	          Assoc_add($$, NULL, $1);
		 }
	  | value_list ' ' value
	         {
		  $$=$1;
		  Assoc_add($$, NULL, $3);
	         }
;
	  
value: STRING 
              {
               $$=CfgValue_create();
	       $$->type = CFG_STRING;
	       $$->value = (void *) $1;
              }
     | INTEGER
              {
               $$=CfgValue_create();
	       $$->type = CFG_INTEGER;
	       $$->value = (void *) $1;
	      }
;

%%

int yyerror(char *s) {
 return 0;
}
