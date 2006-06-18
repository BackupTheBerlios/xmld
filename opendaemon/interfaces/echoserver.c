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

#include "echoserver.h"

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
   _add_error(es_if, "MaxMsgLength configuration directive was not found, using default value.", ERROR_WARNING);
  }
  else {
   ((EchoServerData *) es_if->data)->max_msg_length = (int) es_value->value;
  }
 }

 Module *mcet_module = modman_load_module("mcet", MODULE_CONNECTOR_MODULE);
 ((EchoServerData *) es_if->data)->mcet = (Connector *) modman_get_module_instance(mcet_module, "echoserver_mcet.conf");
 if (((*mcet->init) (((EchoServerData *) es_if->data)->mcet, echoserver_connection, echoserver_request, echoserver_free_user)) == SUCCESS) {
  
  es_directive = CfgSection_get_directive(es_if->cfg, "Listeners", 0);
  if (es_directive != NULL) {
   int i = 0;
   while ((es_value = CfgDirective_get_value(es_directive, i)) != NULL) {
    if (es_value->type != CFG_INTEGER) {
     /* Issue a warning */
    }
    else {
     if ((*(((EchoServerData *) es_if->data)->mcet->add_listener) (((EchoServerData *) es_if->data)->mcet, (int) es_value->value)) == FAILURE) {
      /* Issue a warning */
     }
    }
    i++;
   }
  }

 }
 else {
  /* Issue a fatal error */
  return FAILURE;
 }
}

void echoserver_main(Interface *es_if) {
 Connector *mcet = ((EchoServerData *) es_if->data)->mcet;
 *(mcet->run) (mcet);
}

void echoserver_destroy(Interface *es_if) {
 Connector *mcet = ((EchoServerData *) es_if->data)->mcet;
 *(mcet->destroy) (mcet);
}

void echoserver_connection(Connector *mcet, int fd) {
 Connector *mcet = ((EchoServerData *) es_if->data)->mcet;
 int cfd = socket_accept(fd);
 *(mcet->add_client) (mcet, NULL, cfd);
}

void echoserver_request(Connector *mcet, int fd) {
 Connector *mcet = ((EchoServerData *) es_if->data)->mcet;
 char *msg = socket_read(fd, ((EchoServerData *) es_if->data)->max_msg_length);
 if (strcmp(msg, "stop") == 0) {
  *(mcet_stop) (mcet);
 }
 socket_write(fd, msg);
}

void echoserver_free_user(UserData *data) {
}
