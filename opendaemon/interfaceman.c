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

#include <pwd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "includes.h"
#include "process_pool.h"
#include "opendaemon.h"
#include "classes/interface.h"
#include <errno.h>

int code_len = 0;
int conf_len = 0;
int user_len = 0;

Status interfaceman_init(void) {
 key_t queue_key = ftok("opendaemon.h", 'X');
 int queue_id = msgget(queue_key, IPC_CREAT);

 struct msgbuf buf;

 if (queue_id == -1) {
  printf("\t* Interface manager: Unable to initialize a message queue for the interface manager.\n");
  return FAILURE;
 }

 CfgSection *interface_section;
 CfgDirective *interface_directive;
 CfgValue *interface_value;
 int i = 0;

 while ((interface_section = CfgSection_get_section(cfg_tree, "Interface", i)) != NULL) {
  interface_directive = CfgSection_get_directive(interface_section, "Code", 0);
  if (interface_directive == NULL) {
   printf("\t* Interface manager: Skipping an interface that does not have a Code directive.\n");
   continue;
  }

  interface_value = CfgDirective_get_value(interface_directive, 0);
  if (interface_value == NULL || interface_value->type != CFG_STRING) {
   printf("\t* Interface manager: Skipping an interface with an undefined or malformed Code directive value.\n");
   continue;
  }
  
  buf.mtype = MTYPE_CODE;
  code_len = strlen((char *) interface_value->value);
  strncpy(buf.mtext, (char *) interface_value->value, MAX_MSG_LEN);
  buf.mtext[MAX_MSG_LEN - 1] = '\0';
  msgsnd(queue_id, &buf, (code_len + 1), 0);
  
  interface_directive = CfgSection_get_directive(interface_section, "Conf", 0);
  if (interface_directive != NULL) {
   interface_value = CfgDirective_get_value(interface_directive, 0);
   if (interface_value->type != CFG_STRING) {
    interface_value = NULL;
   }
  }
  else {
   interface_value = NULL;
  }
  
  buf.mtype = MTYPE_CONF;
  if (interface_value != NULL) {
   conf_len = strlen((char *) interface_value->value);
   strncpy(buf.mtext, (char *) interface_value->value, MAX_MSG_LEN);
   buf.mtext[MAX_MSG_LEN - 1] = '\0';
  }

  msgsnd(queue_id, &buf, (conf_len + 1), 0);
  
  interface_directive = CfgSection_get_directive(interface_section, "User", 0);
  if (interface_directive != NULL) {
   interface_value = CfgDirective_get_value(interface_directive, 0);
   if (interface_value->type != CFG_STRING) {
    interface_value = NULL;
   }
  }
  else {
   interface_value = NULL;
  }
  
  if (interface_value != NULL) {
   buf.mtype = MTYPE_USER;
   user_len = strlen((char *) interface_value->value);
   strncpy(buf.mtext, (char *) interface_value->value, MAX_MSG_LEN);
   buf.mtext[MAX_MSG_LEN - 1] = '\0';
  }

  msgsnd(queue_id, &buf, (user_len + 1), 0);
  
  process_pool_handle(main_proc_pool, _launch, (void *) NULL);
  i++;
 }
 return SUCCESS;
}

void _launch(void *null) {
 char *code;
 char *conf = NULL;
 char *user = NULL;

 key_t queue_key = ftok("opendaemon.h", 'X');
 int queue_id = msgget(queue_key, 0);
 struct msgbuf buf;

 buf.mtype = MTYPE_CODE;
 code_len = msgrcv(queue_id, &buf, MAX_MSG_LEN * sizeof(char), MTYPE_CODE, 0);
 code = (char *) malloc(code_len);
 strcpy(code, buf.mtext);
sleep(50); 
 buf.mtype = MTYPE_CONF;
 conf_len = msgrcv(queue_id, &buf, MAX_MSG_LEN * sizeof(char), MTYPE_CONF, 0);
 if (conf_len != -1) {
  conf = (char *) malloc(conf_len + 1);
  strcpy(conf, buf.mtext);
 }

 buf.mtype = MTYPE_USER;
 user_len = msgrcv(queue_id, &buf, MAX_MSG_LEN * sizeof(char), MTYPE_USER, 0);
 if (user_len != -1) {
  user = (char *) malloc(user_len + 1);
  strcpy(user, buf.mtext);
 }

 if (user != NULL) {
  struct passwd *user_id_struct = getpwnam(user);
  if (setuid(user_id_struct->pw_uid) != 0) {
   printf("\t* Interface manager: Unable to set the user to %s for the process of an interface loading the code %s\n", user, code);
   return;
  }
 }

 Module *mod = modman_load_module(code, MODULE_INTERFACE_MODULE);
 if (mod == NULL) {
  printf("\t* Interface manager: Error loading interface module %s\n", code);
  return;
 }

 Interface *if_inst = (Interface *) modman_get_module_instance(mod, conf);
 Error *msg;

 if ((*(if_inst->init)) (if_inst) == FAILURE) {
  msg = (*if_inst->get_error) (if_inst);
  printf("\t* Interface manager: Error initializing interface module %s: %s\n", code, msg->message);
  return;
 }
 else {
  msg = (*if_inst->get_error) (if_inst);
  printf("\t* %s: %s", code, msg->message);
  (*(if_inst->main)) (if_inst); /* Let her do her job */
  (*(if_inst->destroy)) (if_inst); /* Kill her! */
 }

 modman_destroy_module_instance(mod, (void *) if_inst);
}
