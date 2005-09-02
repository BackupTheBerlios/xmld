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
 
Status interface_list_init() {
 Assoc *tmp_list=Assoc_create();
 Interface *curr_interface;
#ifdef USE_ECHOSERVER
 curr_interface=Interface_create();
 curr_interface->init=echoserver_init;
 curr_interface->destroy=echoserver_destroy;
 curr_interface->user_connection=echoserver_user_connection;
 curr_interface->get_error_message=echoserver_get_error_message;
 Assoc_add(tmp_list, "EchoServer", curr_interface);
#endif /* USE_ECHOSERVER */
 
 interface_list = Assoc_create(); 
 CfgSection *ports_section = CfgSection_get_section(cfg_tree, "Ports", 0);
 
 if (ports_section == NULL) {
  return FAILURE;
 }
 
 CfgDirective *port_directive;
 CfgValue *port_value;
 CfgValue *interface_name;
 CfgValue *new_interface_name;
 Status stat;
 int num = 0;
 while ((port_directive = CfgSection_get_directive(ports_section, "Port", num)) != NULL) {
  port_value = CfgDirective_get_value(port_directive, 0);
  if (port_value->type != CFG_INTEGER) {
   continue;
  }
  interface_name = CfgDirective_get_value(port_directive, 1);
  if (interface_name->type != CFG_STRING) {
   continue;
  }
  new_interface_name = CfgDirective_get_value(port_directive, 2);
  if (new_interface_name->type != CFG_STRING) {
   continue;
  }
  curr_interface = Assoc_get(tmp_list, (char *) interface_name->value);
  if (curr_interface == NULL) {
   continue;
  }

  Interface *new_interface = Interface_create();
  Interface_copy(curr_interface, new_interface);
  
  Assoc_add(interface_list, (char *) new_interface_name->value, new_interface);
  stat = (*new_interface->init) (new_interface, port_directive);
  if (stat == FAILURE) {
   printf("Error initializing %s: %s", (char *) new_interface_name->value, (*new_interface->get_error_message) (new_interface));
   Assoc_remove(interface_list, (char *) new_interface_name->value);
   Interface_free(new_interface);
  }
  else {
   new_interface->port = (int) port_value->value;
   printf("\t* %s is on port %d as %s\n", (char *) interface_name->value, new_interface->port, (char *) new_interface_name->value);
  }
  num++;
 }
 AssocWalker *walker = AssocWalker_create(tmp_list);
 while (AssocWalker_next(walker)) {
  Interface_free((Interface *) AssocWalker_get_current_data(walker));
 }
 AssocWalker_free(walker);
 Assoc_free(tmp_list);
 return SUCCESS;
}

Status interface_list_shutdown() {
 AssocWalker *walker = AssocWalker_create(interface_list);
 while (AssocWalker_next(walker)) {
  (*(((Interface *) AssocWalker_get_current_data(walker))->destroy)) ((Interface *) AssocWalker_get_current_data(walker));
  Interface_free((Interface *) AssocWalker_get_current_data(walker));
 }
 AssocWalker_free(walker);
 Assoc_free(interface_list);
 return SUCCESS;
}

Interface *interface_list_search_by_port(int port) {
 AssocWalker walker;
 walker.subject = interface_list;
 walker.curr_index = -1;
 while (AssocWalker_next(&walker)) {
  if (((Interface *) AssocWalker_get_current_data(&walker))->port == port) {
   return (Interface *) AssocWalker_get_current_data(&walker);
  }
 }
 return NULL;
}
