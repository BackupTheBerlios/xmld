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
#include "interface_list.h"

#ifdef USE_XMLDSQL
#include "xmldsql/xmldsql.h"
#endif /* USE_XMLDSQL */
 
XMLDStatus interface_list_init() {
 XMLDAssoc *tmp_list=XMLDAssoc_create();
 XMLDInterface *curr_interface;
#ifdef USE_XMLDSQL
 curr_interface=XMLDInterface_create();
 curr_interface->init=xmldsql_init;
 curr_interface->destroy=xmldsql_destroy;
 curr_interface->prepare_conn=xmldsql_prepare_conn;
 curr_interface->cleanup_conn=xmldsql_cleanup_conn;
 curr_interface->prepare=xmldsql_prepare;
 curr_interface->destroy=xmldsql_destroy;
 curr_interface->parse=xmldsql_parse;
 curr_interface->walk=xmldsql_walk;
 curr_interface->get_response=xmldsql_get_response;
 XMLDAssoc_add(tmp_list, "XMLDSQL", curr_interface);
#endif /* USE_XMLDSQL */
 
 interface_list = XMLDAssoc_create(); 
 XMLDCfgSection *ports_section = XMLDCfgSection_get_section(cfg_tree, "Ports", 1);
 if (ports_section == NULL) {
  return XMLD_FAILURE;
 } 
 XMLDCfgDirective *port_directive;
 XMLDCfgValue *port_value;
 XMLDCfgValue *interface_name;
 XMLDInterface *add_interface;
 num = 1;
 while ((port_directive = XMLDCfgSection_get_directive(ports_section, "Port", num)) != NULL) {
  port_value = XMLDCfgDirective_get_value(port_directive, 0);
  if (port_value->type != XMLD_CFG_INTEGER) {
   continue;
  }
  interface_name = XMLDCfgDirective_get_value(port_directive, 1);
  if (interface_value->type != XMLD_CFG_STRING) {
   continue;
  }
  curr_interface = XMLDAssoc_get(tmp_list, (char *) interface_name->value);
  if (curr_interface == NULL) {
   continue;
  }
  XMLDAssoc_add(interface_list, (char *) interface_name->value, curr_interface);
  (*curr_interface->init) (curr_interface, port_directive);
  curr_interface->port = (int) port_value->value;
  num++;
 } 
 XMLDAssoc_free(tmp_list);
 return XMLD_SUCCESS;
}

XMLDStatus interface_list_shutdown() {
 interface_list_walker = XMLDAssocWalker_create(interface_list);
 while (XMLDAssocWalker_next(interface_list_walker)) {
  (*(((XMLDInterface *) XMLDAssocWalker_get_current_data(interface_list_walker))->destroy)) ();
  XMLDInterface_free((XMLDInterface *) XMLDAssocWalker_get_current_data(interface_list_walker));
 }
 XMLDAssoc_free(interface_list);
 return XMLD_SUCCESS;
}
