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
  document_root = (char *) ((XMLDList_first(cfg_get(NULL, "DocumentRoot", NULL)))->value);
  if (document_root == NULL) {
   return XMLD_FAILURE;
  }
  return XMLD_SUCCESS;
 }
}

/*
 * Gets the value of the given directive in the given
 * configuration subsection of the given tree
 * structure.
 *
 * NULL for section means getting a directive from 
 * the given tree's root.
 *
 * NULL for the tree means getting the directive from
 * cfg_tree.
 */
XMLDCfgValueList *cfg_get(char *section, char *directive, XMLDCfgSection *tree) {
 if (tree == NULL) {
  tree = cfg_tree;
 }
 XMLDCfgSection *tmp;
 if (section == NULL) {
  tmp = cfg_tree;
 }
 else {
  tmp = XMLDCfgSectionList_search_by_name(tree->sections, section);
 }
 if (tmp == NULL) {
  return NULL;
 }
 XMLDCfgDirective *tmp_dir = XMLDCfgDirectiveList_search_by_name(tmp->directives, directive);
 if (tmp_dir == NULL) {
  return NULL;
 }
 return tmp_dir->values;
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
XMLDStatus cfg_shutdown() {
 XMLDCfgSection_free(cfg_tree);
 return XMLD_SUCCESS;
}

