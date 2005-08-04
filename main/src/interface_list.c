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

#ifdef USE_ECHOSERVER
#include "echoserver/echoserver.h"
#endif /* USE_ECHOSERVER */
 
XMLDStatus interface_list_init() {
 XMLDAssoc *tmp_list=XMLDAssoc_create();
 XMLDInterface *curr_interface;
#ifdef USE_ECHOSERVER
 curr_interface=XMLDInterface_create();
 curr_interface->init=echoserver_init;
 curr_interface->destroy=echoserver_destroy;
 curr_interface->user_connection=echoserver_user_connection;
 curr_interface->get_error_message=echoserver_get_error_message;
 XMLDAssoc_add(tmp_list, "EchoServer", curr_interface);
#endif /* USE_ECHOSERVER */
 
 interface_list = XMLDAssoc_create(); 
 XMLDCfgSection *ports_section = XMLDCfgSection_get_section(cfg_tree, "Ports", 1);
 if (ports_section == NULL) {
  return XMLD_FAILURE;
 } 
 XMLDCfgDirective *port_directive;
 XMLDCfgValue *port_value;
 XMLDCfgValue *interface_name;
 XMLDStatus stat;
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
  stat = (*curr_interface->init) (curr_interface, port_directive);
  if (stat == XMLD_FAILURE) {
   perror("Error initializing %s: %s", (char *) interface_name->value, (*curr_interface->get_error_message) (curr_interface));
   XMLDAssoc_remove_index(interface_list, (char *) interface_name->value);
   XMLDInterface_free(curr_interface);
  }
  else {
   curr_interface->port = (int) port_value->value;
  }
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

XMLDInterface *interface_list_search_by_port(int port) {
 XMLDAssocWalker walker;
 walker.subject = interface_list;
 walker.curr_index = -1;
 while (XMLDAssocWalker_next(&walker)) {
  if (((XMLDInterface *) XMLDAssocWalker_get_current_data(&walker))->port == port) {
   return (XMLDInterface *) XMLDAssocWalker_get_current_data(&walker);
  }
 }
}
