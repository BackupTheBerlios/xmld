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
#include "xmld_cfg_value.h"
#include "xmld_cfg_directive.h"
#include "xmld_cfg_section.h"
#include "cfg_parser.h"

int yyparse(void *);
int yyin;

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
void yy_delete_buffer(YY_BUFFER_STATE);

/*
 * : Parses the configuration file and makes
 * configuration directives be available for 
 * retrieval. (this function is a small lexer
 * which uses cfg_parser_parse_token as its
 * parser)
 * returns: whether successful.
 */
XMLDStatus cfg_parser_parse() {
 errno=0;
 FILE *conf=fopen("xmld.conf", "r");

 if (errno != 0) {
  return XMLD_FAILURE;
 }

 YY_BUFFER_STATE buf = yy_create_buffer(conf, YY_BUF_SIZE);
 XMLDStatus status = yyparse(cfg_tree);
 yy_delete_buffer(buf);
 if (status == -1) {
  return XMLD_FAILURE;
 }
 else {
  return XMLD_SUCCESS;
 }
}

/*
 * : Frees up resources used by the parse tree
 * created by cfg_parser_parse, and removes
 * configuration directives from memory.
 * returns: whether successful.
 */
XMLDStatus cfg_parser_clean() {
 XMLDCfgSection_free(cfg_tree);
 return XMLD_SUCCESS;
}
