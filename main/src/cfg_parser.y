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
           $$->directives = XMLDCfgDirectiveList_create();
           XMLDCfgDirective *directive = XMLDCfgDirectiveList_add($$->directives);
	   directive->name = $1->name;
	   directive->values = $1->values;
	   free($1);
	  }
	| section
	  {
	   $$=XMLDCfgSection_create();
	   $$->sections = XMLDCfgSectionList_create();
	   XMLDCfgSection *section = XMLDCfgSectionList_add($$->sections);
	   section->name = $1->name;
	   section->directives = $1->directives;
	   free($1);
	  }
	| cfg_tree '\n' directive
	  {
	   $$=$1;
           XMLDCfgDirective *directive = XMLDCfgDirectiveList_add($$->directives);
	   directive->name = $3->name;
	   directive->values = $3->values;
	   free($3);
	  }
	| cfg_tree '\n' section 
	  {
	   $$=$1;
	   XMLDCfgSection *section = XMLDCfgSectionList_add($$->sections);
	   section->name = $3->name;
	   section->directives = $3->directives;
	   free($3);
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
