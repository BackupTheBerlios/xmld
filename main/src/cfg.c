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
#include "cfg.h"

int yyparse(void *);

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
void yy_delete_buffer(YY_BUFFER_STATE);

/*
 * : Initiates the configuration manager.
 * returns: whether successful.
 */
XMLDStatus cfg_init() {
 cfg_tree=NULL;
 errno=0;
 FILE *conf=fopen("opendaemon.conf", "r");

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
  XMLDCfgDirective *docroot_directive = XMLDCfgSection_get_directive(cfg_tree, "DocumentRoot", 1);
  if (docroot_directive == NULL) {
   return XMLD_FAILURE;
  }
  XMLDCfgValue *docroot_value = XMLDCfgDirective_get_value(docroot_directive);
  if (docroot_value->type != XMLD_CFG_STRING) {
   return XMLD_FAILURE;
  }
  document_root = (char *) docroot_value->value;
  return XMLD_SUCCESS;
 }
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
XMLDStatus cfg_shutdown() {
 XMLDCfgSection_free(cfg_tree);
 return XMLD_SUCCESS;
}

void cfg_update() {
 cfg_shutdown();
 cfg_init();
}

