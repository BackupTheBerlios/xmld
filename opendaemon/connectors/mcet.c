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
#include "../def.h"
#include "../classes/connector.h"
#include <unistd.h>
#include <fcntl.h>
#include "mcet.h"

void *_get_module_instance(CfgTree *cfg) {
 Connector *mcet = (Connector *) malloc(sizeof(Connector));
 mcet->cfg = cfg;
 mcet->init = mcet_init;
 mcet->set_connection_handler = mcet_set_connection_handler;
 mcet->set_request_handler = mcet_set_request_handler;
 mcet->add_listener = mcet_add_listener;
 mcet->remove_listener = mcet_remove_listener;
 mcet->add_client = mcet_add_client;
 mcet->remove_client = mcet_remove_client;
 mcet->run = mcet_run;
 mcet->destroy = mcet_destroy;
 mcet->get_error_message = mcet_get_error_message;
 return mcet;
}

Status mcet_init(Connector *mcet, UserData *(*conn_handler) (Connector *, int), Response *(*req_handler) (Connector *, int)) {
 mcet->data = malloc(sizeof(MCETData));
 ((MCETData *) mcet->data)->conn_handler = conn_handler;
 ((MCETData *) mcet->data)->req_handler = req_handler;
 ((MCETData *) mcet->data)->socks = Assoc_create();
 ((MCETData *) mcet->data)->socks->integer_keys = TRUE;
 ((MCETData *) mcet->data)->infinite_run = FALSE;
 ((MCETData *) mcet->data)->run_iterations = 2;
 ((MCETData *) mcet->data)->timeout_seconds = 0;
 ((MCETData *) mcet->data)->timeout_microseconds = 500000;
 FD_ZERO(&(((MCETData *) mcet->data)->tmp_set));
 ((MCETData *) mcet->data)->intact_fd_set = FALSE;

 CfgDirective *conf_directive;
 CfgValue *conf_value;
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "InfiniteRun", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_STRING) {
   if (strcmp(conf_value->value, "yes") == 0) {
    ((MCETData *) mcet->data)->infinite_run = TRUE;
   }
   else {
    ((MCETData *) mcet->data)->infinite_run = FALSE;
   }
  }
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "RunIterations", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->run_iterations = (int) conf_value->value;
  }
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "TimeoutSeconds", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->timeout_seconds = (int) conf_value->value;
  }
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "TimeoutMicroseconds", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->timeout_microseconds = (int) conf_value->value;
  }
 }
 
 int init_threads = 5, max_threads = 20, max_idle_threads = 10;
 
 CfgSection *pool_section = CfgSection_get_section(mcet->cfg, "ThreadPool", 0);
 
 if ((conf_directive = CfgSection_get_directive(pool_section, "InitThreads", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   init_threads = (int) conf_value->value;
  }
 }
 
 if ((conf_directive = CfgSection_get_directive(pool_section, "MaxThreads", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   max_threads = (int) conf_value->value;
  }
 }
 
 if ((conf_directive = CfgSection_get_directive(pool_section, "MaxIdleThreads", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   max_idle_threads = (int) conf_value->value;
  }
 }
 
 ((MCETData *) mcet->data)->executor_pool = thread_pool_create(init_threads, max_threads, max_idle_threads);
 
 if (((MCETData *) mcet->data)->executor_pool != NULL) {
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

void mcet_set_connection_handler(Connector *mcet, UserData *(*conn_handler) (void)) {
 ((MCETData *) mcet->data)->conn_handler = conn_handler;
}

void mcet_set_request_handler(Connector *mcet, Response *(*req_handler) (Request *, UserData *)) {
 ((MCETData *) mcet->data)->req_handler = req_handler;
}

Status mcet_add_listener(Connector *mcet, int fd) {
 fcntl(fd, F_SETFL, O_NONBLOCK|fcntl(fd, F_GETFL));
 if (Assoc_get(((MCETData *) mcet->data)->socks, (void *) fd) == NULL) {
  Assoc_add(((MCETData *) mcet->data)->socks, (void *) fd, (void *) 1);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

Status mcet_remove_listener(Connector *mcet, int fd) {
 if (Assoc_remove(((MCETData *) mcet->data)->socks, (void *) fd) == SUCCESS);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

Status mcet_add_client(Connector *mcet, UserData *data, int fd) {
 if (Assoc_get(((MCETData *) mcet->data)->socks, (void *) fd) == NULL) {
  Assoc_add(((MCETData *) mcet->data)->socks, (void *) fd, data);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

Status mcet_remove_client(Connector *mcet, int fd) {
 if (Assoc_remove(((MCETData *) mcet->data)->socks, (void *) fd) == SUCCESS);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

Status mcet_run(Connector *mcet) {
 int sock = -1, num_sock = 0, iterations = 0;
 struct timeval tv;
 fd_set sock_set;
 AssocWalker walker;
 walker.subject = ((MCETData *) mcet->data)->socks;
 walker.curr_index = -1;
 
 while (((MCETData *) mcet->data)->infinite_run == TRUE || iterations < ((MCETData *) mcet->data)->run_iterations) {
  tv.tv_sec = ((MCETData *) mcet->data)->timeout_seconds;
  tv.tv_usec = ((MCETData *) mcet->data)->timeout_microseconds;
  
  if (((MCETData *) mcet->data)->intact_fd_set == FALSE) {
   ((MCETData *) mcet->data)->n = 0;
   FD_ZERO(&sock_set);
   FD_ZERO(&(((MCETData *) mcet->data)->tmp_set));
   AssocWalker_reset(&walker);
   while (AssocWalker_next(&walker) != ASSOC_WALKER_END) {
    sock = AssocWalker_get_current_key(&walker);
    FD_SET(sock, &sock_set);
    FD_SET(sock, &(((MCETData *) mcet->data)->tmp_set));
    ((MCETData *) mcet->data)->n = (sock > ((MCETData *) mcet->data)->n) ? sock : ((MCETData *) mcet->data)->n;
   }
   ((MCETData *) mcet->data)->intact_fd_set = TRUE;
  }
  else {
   memcpy(&sock_set, &(((MCETData *) mcet->data)->tmp_set), sizeof(fd_set));   
  }

  num_sock = select(((MCETData *) mcet->data)->n, sock_set, NULL, NULL, &tv);
  
  void *arr[2];
  void *sock_data;
  AssocWalker_reset(&walker);
  while (AssocWalker_next(&walker) != ASSOC_WALKER_END && num_sock > 0)  {
   sock = AssocWalker_get_current_key(&walker);
   if (FD_ISSET(sock, &sock_set)) {
    arr[0] = (void *) mcet;
    arr[1] = (void *) sock;
    sock_data = AssocWalker_get_current_data(&walker);
    if (sock_data == (void *) 1) {
     thread_pool_handle(((MCETData *) mcet->data)->executor_pool, _conn_handler, (void *) arr);
    }
    else if (sock_data != NULL) {
     thread_pool_handle(((MCETData *) mcet->data)->executor_pool, _req_handler, (void *) arr);
    }
    num_sock--;
   }
  }
  iterations++;
 }
}

void _conn_handler(void *params) {
 arr = (void **) params;
 *(((MCETData *) mcet->data)->conn_handler) ((Connector *) arr[0], (int) arr[1]);
}

void _req_handler(void *params) {
 arr = (void **) params;
 *(((MCETData *) mcet->data)->req_handler) ((Connector *) arr[0], (int) arr[1]);
}
