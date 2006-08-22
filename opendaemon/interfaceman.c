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

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include "includes.h"
#include "process_pool.h"
#include "opendaemon.h"
#include "classes/interface.h"

Status interfaceman_init(void) {
 CfgSection *interface_section;
 CfgDirective *interface_directive;
 CfgValue *interface_params[3];
 int i = 0;

 while ((interface_section = CfgSection_get_section(cfg_tree, "Interface", i)) != NULL) {
  interface_directive = CfgSection_get_directive(interface_section, "Code", 0);
  if (interface_directive == NULL) {
   printf("\t* Interface manager: Skipping an interface that does not have a Code directive.\n");
   continue;
  }

  interface_params[0] = CfgDirective_get_value(interface_directive, 0);
  if (interface_params[0] == NULL || interface_params[0]->type != CFG_STRING) {
   printf("\t* Interface manager: Skipping an interface with an undefined or malformed Code directive value.\n");
   continue;
  }
  
  interface_directive = CfgSection_get_directive(interface_section, "Conf", 0);
  if (interface_directive != NULL) {
   interface_params[1] = CfgDirective_get_value(interface_directive, 0);
   if (interface_params[1]->type != CFG_STRING) {
    interface_params[1] = NULL;
   }
  }
  else {
   interface_params[1] = NULL;
  }
  
  interface_directive = CfgSection_get_directive(interface_section, "User", 0);
  if (interface_directive != NULL) {
   interface_params[2] = CfgDirective_get_value(interface_directive, 0);
   if (interface_params[2]->type != CFG_STRING) {
    interface_params[2] = NULL;
   }
  }
  else {
   interface_params[2] = NULL;
  }
  
  process_pool_handle(main_proc_pool, _launch, (void *) &interface_params);
  i++;
 }
 return SUCCESS;
}

void _launch(void *params) {
 CfgValue **cfg_params = (CfgValue **) params;
 if (*(cfg_params + 2) != NULL) {
  struct passwd *user_id_struct = getpwnam((*(cfg_params + 2))->value);
  if (setuid(user_id_struct->pw_uid) != 0) {
   printf("\t* Interface manager: Unable to set the user to %s for the process of an interface loading the code %s\n", (char *) (*(cfg_params + 2))->value, (char *) (*(cfg_params))->value);
   return;
  }
 }

 Module *mod = modman_load_module((*cfg_params)->value, MODULE_INTERFACE_MODULE);
 if (mod == NULL) {
  printf("\t* Interface manager: Error loading interface module %s\n", (char *) (*(cfg_params))->value);
  return;
 }

 Interface *if_inst = (Interface *) modman_get_module_instance(mod, (char *) (*(cfg_params + 1))->value);
 Error *msg;

 if ((*(if_inst->init)) (if_inst) == FAILURE) {
  msg = (*if_inst->get_error) (if_inst);
  printf("\t* Interface manager: Error initializing interface module %s: %s\n", (char *) (*(cfg_params))->value, msg->message);
  return;
 }
 else {
  msg = (*if_inst->get_error) (if_inst);
  printf("\t* %s: %s", (char *) (*(cfg_params))->value, msg->message);
  (*(if_inst->main)) (if_inst); /* Let her do her job */
  (*(if_inst->destroy)) (if_inst); /* Kill her! */
 }

 modman_destroy_module_instance(mod, (void *) if_inst);
}
