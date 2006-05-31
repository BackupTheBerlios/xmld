/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#ifndef __ECHOSERVER_H
#define __ECHOSERVER_H

void *_get_module_instance(CfgSection *tree) {
 Interface *es_if = (Interface *) malloc(sizeof(Interface));
 es_if->cfg = tree;
 es_if->init = echoserver_init;
 es_if->main = echoserver_main;
 es_if->destroy = echoserver_destroy;
 es_if->get_error = echoserver_get_error;
 return es_if;
}

Status echoserver_init(Interface *es_if) {
 es_if->data = (InterfaceData *) malloc(sizeof(EchoServerData));
 ((EchoServerData *) es_if->data)->max_msg_length = 100; /* Default value */

 CfgDirective *es_directive = CfgSection_get_directive(es_if->cfg, "MaxMsgLength", 0);
 CfgValue *es_value;

 if (es_directive != NULL) {
  es_value = CfgDirective_get_value(es_directive, 0);
  if (es_value == NULL || es_value->type != CFG_INTEGER) {
   /* Issue a warning */
  }
  else {
   ((EchoServerData *) es_if->data)->max_msg_length = (int) es_value->value;
  }
 }

 Module *mcet_module = modman_load_module("mcet", MODULE_CONNECTOR_MODULE);
 /* shall we check for module status */
 Connector *mcet = (Connector *) modman_get_module_instance(mcet_module, "echoserver_mcet.conf");
 (*mcet->init) (mcet, echoserver_connection, echoserver_request, echoserver_free_user);
 /* Add listeners according to conf */ 
}

void echoserver_main(Interface *es_if) {
 /* mcet_run */
}

void echoserver_destroy(Interface *es_if) {
}

void echoserver_connection(Connector *mcet, int fd) {
}

void echoserver_request(Connector *mcet, int fd) {
}

void echoserver_free_user(UserData *data) {
}

#endif /* __ECHOSERVER_H */
