/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#include <errno.h>
#include "includes.h"

int yyparse(void *);
int yywrap(void);
int yerror(char *);

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *);
void yy_switch_to_buffer(YY_BUFFER_STATE);
void yy_delete_buffer(YY_BUFFER_STATE);
YY_BUFFER_STATE yy_create_buffer(FILE*, int);

/*
 * : Initiates the configuration manager.
 * returns: whether successful.
 */
Status cfg_init() {
 cfg_tree = cfg_create_tree("opendaemon.conf");
 
 if (cfg_tree == NULL) {
  return FAILURE;
 }
 else {
  CfgDirective *serverroot_directive = CfgSection_get_directive(cfg_tree, "ServerRoot", 0);
  if (serverroot_directive == NULL) {
   return FAILURE;
  }
  CfgValue *serverroot_value = CfgDirective_get_value(serverroot_directive, 0);
  if (serverroot_value->type != CFG_STRING) {
   return FAILURE;
  }
  server_root = (char *) serverroot_value->value;
  return SUCCESS;
 }
}

/*
 * Creates a configuration tree out of the given configuration file.
 */
CfgSection *cfg_create_tree(char *file) {
 CfgSection *ret = NULL;
 errno=0;
 FILE *conf=fopen(file, "r");

 if (errno != 0) {
  return NULL;
 }

 YY_BUFFER_STATE buf = yy_create_buffer(conf, YY_BUF_SIZE);
 yy_switch_to_buffer(buf);
 Status status = yyparse(&ret);
 yy_delete_buffer(buf);

 if (status == -1) {
  return NULL;
 }
 else {
  return ret;
 }
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
Status cfg_shutdown() {
 cfg_destroy_tree(cfg_tree);
 return SUCCESS;
}

/*
 * Finalizes and cleans up the given parse tree.
 * returns: whether successful.
 */
Status cfg_destroy_tree(CfgSection *tree) {
 CfgSection_free(tree);
 return SUCCESS;
}
