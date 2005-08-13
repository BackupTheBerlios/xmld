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
Status cfg_init() {
 cfg_tree=NULL;
 errno=0;
 FILE *conf=fopen("opendaemon.conf", "r");

 if (errno != 0) {
  return FAILURE;
 }

 YY_BUFFER_STATE buf = yy_create_buffer(conf, YY_BUF_SIZE);
 Status status = yyparse(cfg_tree);
 yy_delete_buffer(buf);
 if (status == -1) {
  return FAILURE;
 }
 else {
  CfgDirective *docroot_directive = CfgSection_get_directive(cfg_tree, "DocumentRoot", 1);
  if (docroot_directive == NULL) {
   return FAILURE;
  }
  CfgValue *docroot_value = CfgDirective_get_value(docroot_directive);
  if (docroot_value->type != CFG_STRING) {
   return FAILURE;
  }
  document_root = (char *) docroot_value->value;
  return SUCCESS;
 }
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
Status cfg_shutdown() {
 CfgSection_free(cfg_tree);
 return SUCCESS;
}

void cfg_update() {
 cfg_shutdown();
 cfg_init();
}

