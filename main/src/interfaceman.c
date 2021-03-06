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
#include "interfaceman.h"

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
  
  ptasker_handle(main_proc_pool, _launch, (void *) &interface_params);
  i++;
 }
 return SUCCESS;
}

void _launch(void *params) {
 CfgValue **cfg_params = (CfgValue **) params;
 if (*(cfg_params + 2) != NULL) {
  struct passwd *user_id_struct = getpwnam((*(cfg_params + 2))->value);
  if (setuid(user_id_struct->pw_uid) != 0) {
   printf("\t* Interface manager: Unable to set the user to %s for the process of an interface loading the code %s\n", (*(cfg_params+2))->value, (*(cfg_params + 1))->value);
   return;
  }
 }

 void *mod_handle = modman_load_module((*(cfg_params + 1))->value, MODMAN_INTERFACE_MODULE);
 if (mod_handle == NULL) {
  printf("\t* Interface manager: Error loading interface module %s\n", (*(cfg_params + 1))->value);
  return;
 }

 Status (*_interface_init) (CfgValue **) = dlsym(mod_handle, "_interface_init");
 
 if (_interface_init == NULL) {
  printf("\t* Interface manager: Could not find an initialization routine in the interface module %s\n", (*(cfg_params + 1))->value);
  return;
 }

 char *_interface_init_msg = dlsym(mod_handle, "_interface_init_msg");
 if ((*(_interface_init)) (cfg_params) == FAILURE) {
  printf("\t* Interface manager: Error initializing interface module %s: %s\n", (*(cfg_params + 1))->value, _interface_init_msg);
  return;
 }
 else {
  printf("\t* %s: %s", (*(cfg_params + 1))->value, _interface_init_msg);
  void (*_interface_main) (void) = dlsym(mod_handle, "_interface_main");
  if (_interface_main == NULL) {
   printf("\t* Interface manager: Could not find a main routine in the interface module %s\n", (*(cfg_params + 1))->value);
   ptasker_kill_me(main_proc_pool);
   exit(1);
   return;
  }
  else {
   (*(_interface_main)) ();
  }
 } 
}
