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
 XMLDList *list;
}

/* Thread-safe */
%pure_parser

/* Types and tokens */
%type <list> value_list directive_list
%type <section> cfg_tree section
%type <directive> directive
%type <value> value
%token <string> IDENTIFIER STRING
%token <integer> INTEGER

%%

cfg_tree: directive_list
	| section
	| cfg_tree '\n' directive
	| cfg_tree '\n' section
;

section: '<' IDENTIFIER '>' '\n' directive_list '\n' '<' '/' IDENTIFIER '>'
         {
	  $$=XMLDCfgSection_create();
	  $$->name = $2;
	  $$->directives = $5;
	 }
;

directive_list: directive
	       {
	        $$=XMLDCfgDirectiveList_create();
		XMLDCfgDirective *directive = XMLDCfgDirectiveList_add($$);
		directive->name = $1->name;
		directive->values = $1->values;
		free($1);
	       }
	      | directive_list '\n' directive
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
		  $$=XMLDCfgValueList_create();
		  XMLDCfgValue *tmp = XMLDCfgValueList_add($$);
		  tmp->type = $1->type;
		  tmp->value = $1->value;
		  free($1);
	         }
	  | value_list ' ' value
	         {
		  $$=$1;
		  XMLDValueCfg *tmp = XMLDCfgValueList_add($$);
		  tmp->type = $3->type;
		  tmp->value = $3->value;
		  free($3);
	         }
;
	  
value: STRING 
              {
               $$=XMLDCfgValue_create();
	       $$->type = XMLD_CFG_STRING;
	       $$->value = (char *) $1;
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
