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
}

/* Thread-safe */
%pure_parser

/* Types */
%token <string> IDENTIFIER STRING
%token <integer> INTEGER

%%

cfg_tree: directive_list
	| section
	| cfg_tree '\n' directive
	| cfg_tree '\n' section
;

section: '<' IDENTIFIER '>' '\n' directive_list '\n' '<' '/' IDENTIFIER '>'
;

directive_list: directive
	      | directive_list '\n' directive
;

directive: IDENTIFIER ' ' value_list
;

value_list: value 
	  | value_list ' ' value
;
	  
value: STRING 
     | INTEGER
;

%%

int yyerror(char *s) {
 return 0;
}
