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
 mcet->modcap = NOCAP;
 mcet->init = mcet_init;
 mcet->set_connection_handler = mcet_set_connection_handler;
 mcet->set_request_handler = mcet_set_request_handler;
 mcet->set_user_data_free_func = mcet_set_user_data_free_func;
 mcet->add_listener = mcet_add_listener;
 mcet->remove_listener = mcet_remove_listener;
 mcet->add_client = mcet_add_client;
 mcet->remove_client = mcet_remove_client;
 mcet->get_client_data = mcet_get_client_data;
 mcet->set_client_data = mcet_set_client_data;
 mcet->run = mcet_run;
 mcet->stop = mcet_stop;
 mcet->destroy = mcet_destroy;
 mcet->get_error = mcet_get_error;
 return mcet;
}

Status mcet_init(Connector *mcet, UserData *(*conn_handler) (Connector *, int), Response *(*req_handler) (Connector *, int), void (*user_data_free_func) (UserData *)) {
 mcet->data = malloc(sizeof(MCETData));
 ((MCETData *) mcet->data)->conn_handler = conn_handler;
 ((MCETData *) mcet->data)->req_handler = req_handler;
 ((MCETData *) mcet->data)->user_data_free_func = user_data_free_func;
 ((MCETData *) mcet->data)->socks = Assoc_create();
 ((MCETData *) mcet->data)->socks->integer_keys = TRUE;
 ((MCETData *) mcet->data)->errors = Assoc_create();
 ((MCETData *) mcet->data)->errors->integer_keys = TRUE; 
 ((MCETData *) mcet->data)->infinite_run = FALSE;
 ((MCETData *) mcet->data)->run_iterations = 2;
 ((MCETData *) mcet->data)->timeout_seconds = 0;
 ((MCETData *) mcet->data)->timeout_microseconds = 500000;
 FD_ZERO(&(((MCETData *) mcet->data)->tmp_set));
 ((MCETData *) mcet->data)->intact_fd_set = FALSE;
 ((MCETData *) mcet->data)->stop = FALSE;

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
 else {
  _add_error(mcet, "InfiniteRun directive not provided, using default value.", ERROR_WARNING);
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "RunIterations", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->run_iterations = (int) conf_value->value;
  }
 }
 else {
  _add_error(mcet, "RunIterations directive not provided, using default value.", ERROR_WARNING);
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "TimeoutSeconds", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->timeout_seconds = (int) conf_value->value;
  }
 }
 else {
  _add_error(mcet, "TimeoutSeconds directive not provided, using default value.", ERROR_WARNING);
 }
 
 if ((conf_directive = CfgSection_get_directive(mcet->cfg, "TimeoutMicroseconds", 0)) != NULL) {
  conf_value = CfgDirective_get_value(conf_directive, 0);
  if (conf_value->type == CFG_INTEGER) {
   ((MCETData *) mcet->data)->timeout_microseconds = (int) conf_value->value;
  }
 }
 else {
  _add_error(mcet, "TimeoutMicroseconds directive not provided, using default value.", ERROR_WARNING);
 }
 
 int init_threads = 5, max_threads = 20, max_idle_threads = 10;
 
 CfgSection *pool_section = CfgSection_get_section(mcet->cfg, "ThreadPool", 0);
 
 if (pool_section == NULL) {
  _add_error(mcet, "ThreadPool configuration section not provided, using default values.", ERROR_WARNING);  
 }
 else {
  if ((conf_directive = CfgSection_get_directive(pool_section, "InitThreads", 0)) != NULL) {
   conf_value = CfgDirective_get_value(conf_directive, 0);
   if (conf_value->type == CFG_INTEGER) {
    init_threads = (int) conf_value->value;
   }
  }
  else {
   _add_error(mcet, "InitThreads directive not provided, using default value.", ERROR_WARNING);
  }
  
  if ((conf_directive = CfgSection_get_directive(pool_section, "MaxThreads", 0)) != NULL) {
   conf_value = CfgDirective_get_value(conf_directive, 0);
   if (conf_value->type == CFG_INTEGER) {
    max_threads = (int) conf_value->value;
   }
  }
  else {
   _add_error(mcet, "MaxThreads directive not provided, using default value.", ERROR_WARNING);
  } 

  if ((conf_directive = CfgSection_get_directive(pool_section, "MaxIdleThreads", 0)) != NULL) {
   conf_value = CfgDirective_get_value(conf_directive, 0);
   if (conf_value->type == CFG_INTEGER) {
    max_idle_threads = (int) conf_value->value;
   }
  }
 }
 else {
  _add_error(mcet, "MaxIdleThreads directive not provided, using default value.", ERROR_WARNING);
 } 

 ((MCETData *) mcet->data)->executor_pool = thread_pool_create(init_threads, max_threads, max_idle_threads);
 
 if (((MCETData *) mcet->data)->executor_pool != NULL) {
  return SUCCESS;
 }
 else {
  _add_error(mcet, "A fatal error has occured while initializing the executor thread pool with the given parameters.", ERROR_FATAL);
  return FAILURE;
 }
}

void mcet_set_connection_handler(Connector *mcet, UserData *(*conn_handler) (void)) {
 ((MCETData *) mcet->data)->conn_handler = conn_handler;
}

void mcet_set_request_handler(Connector *mcet, Response *(*req_handler) (Request *, UserData *)) {
 ((MCETData *) mcet->data)->req_handler = req_handler;
}

void mcet_set_user_data_free_func(Connector *mcet, void (*func) (UserData *)) {
 ((MCETData *) mcet->data)->user_data_free_func = func;
}

Status mcet_add_listener(Connector *mcet, int fd) {
 fcntl(fd, F_SETFL, O_NONBLOCK|fcntl(fd, F_GETFL));
 if (Assoc_get(((MCETData *) mcet->data)->socks, (void *) fd) == NULL) {
  Assoc_add(((MCETData *) mcet->data)->socks, (void *) fd, (void *) 1);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  _add_error(mcet, "Listener already in list", ERROR_WARNING);
  return FAILURE;
 }
}

Status mcet_remove_listener(Connector *mcet, int fd) {
 if (Assoc_remove(((MCETData *) mcet->data)->socks, (void *) fd) == SUCCESS);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  _add_error(mcet, "Listener not in list", ERROR_WARNING);
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
  _add_error(mcet, "Client already in list", ERROR_WARNING);
  return FAILURE;
 }
}

Status mcet_remove_client(Connector *mcet, int fd) {
 if (Assoc_remove(((MCETData *) mcet->data)->socks, (void *) fd) == SUCCESS);
  ((MCETData *) mcet->data)->intact_fd_set = FALSE;
  return SUCCESS;
 }
 else {
  _add_error(mcet, "Client not in list", ERROR_WARNING);
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
 
 while (((MCETData *) mcet->data)->stop == FALSE && (((MCETData *) mcet->data)->infinite_run == TRUE || iterations < ((MCETData *) mcet->data)->run_iterations)) {
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
 ((MCETData *) mcet->data)->stop == FALSE;
 return SUCCESS;
}

Status mcet_stop(Connector *mcet) {
 ((MCETdata *) mcet->data)->stop = TRUE;
 return SUCCESS;
}

UserData *mcet_get_client_data(Connector *mcet, int fd) {
 return Assoc_get(((MCETData *) mcet->data)->socks, (void *) fd);
}
Status mcet_set_client_data(Connector *mcet, UserData *data, int fd) {
 return Assoc_set(((MCETData *) mcet->data)->socks, (void *) fd, data);
}

void _conn_handler(void *params) {
 arr = (void **) params;
 *(((MCETData *) mcet->data)->conn_handler) ((Connector *) arr[0], (int) arr[1]);
}

void _req_handler(void *params) {
 arr = (void **) params;
 *(((MCETData *) mcet->data)->req_handler) ((Connector *) arr[0], (int) arr[1]);
}

void mcet_destroy(Connector *mcet) {
 /* Just to make sure! */
 mcet_stop(mcet);

 /* Destroy the executor thread pool */
 thread_pool_destroy(((MCETData *) mcet->data)->executor_pool);

 /* Free UserData */
 AssocWalker walker;
 walker.subject = ((MCETData *) mcet->data)->socks;
 walker.curr_index = -1;
 
 while (AssocWalker_next(&walker) != ASSOC_WALKER_END) {
  if (AssocWalker_get_current_data(&walker) != (void *) 1) {
   *(((MCETData *) mcet->data)->user_data_free_func) (AssocWalker_get_current_data(&walker));
  }
 }
 
 /* Free Errors */
 walker.subject = ((MCETData *) mcet->data)->errors;
 walker.curr_index = -1;
 
 while (AssocWalker_next(&walker) != ASSOC_WALKER_END) {
  free(AssocWalker_get_current_data(&walker));
 }

 /* Free the data structure */
 free(mcet->data); 
}

Error *mcet_get_error(Connector *mcet) {
 int length = Assoc_get_length(((MCETData *) mcet->data)->errors);

 if (length == 0) {
  return NULL;
 }

 Error *ret = Assoc_get(((MCETData *) mcet->data)->errors, (void *) length);
 _remove_error(mcet);
 return ret;
}

void _add_error(Connector *mcet, char *msg, ErrorLevel level) {
 Error *err = (Error *) malloc(sizeof(Error));
 Error_set(err, msg, level);
 int length = Assoc_get_length(((MCETData *) mcet->data)->errors);
 Assoc_add(((MCETData *) mcet->data)->errors, (void *) length, err);
}

void _remove_error(Connector *mcet) {
 int length = Assoc_get_length(((MCETData *) mcet->data)->errors);
 free(Assoc_get(((MCETData *) mcet->data)->errors, length));
 Assoc_remove(((MCETData *) mcet->data)->errors, (void *) length);
}
