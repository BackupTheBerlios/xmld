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

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "modman.h"
#include "cfg.h"

Status modman_init() {
 CfgSection *modules_section = CfgSection_get_section(cfg_tree, "Modules", 0);
 CfgDirective *module_type_directive;
 CfgValue *module_type_value;

 module_type_directive = CfgSection_get_directive(modules_section, "Interface", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: there is no interface module directory defined.\n");
  return FAILURE;
 }

 module_type_value = CfgSection_get_value(module_type_directive, 0);
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for the interface module directory.\n");
  return FAILURE;
 }
 interfaces_dir = (char *) module_type_value->value;

 module_type_directive = CfgSection_get_directive(modules_section, "Engine", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No engine module directory defined, support for engine loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for engine module directory, support for engine loading disabled.\n");
 }
 engines_dir = (char *) module_type_value->value;
 
 module_type_directive = CfgSection_get_directive(modules_section, "Processor", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No processor module directory defined, support for processor loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for processor module directory, support for processor loading disabled.\n");
 }
 processors_dir = (char *) module_type_value->value;

 module_type_directive = CfgSection_get_directive(modules_section, "Executor", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No executor module directory defined, support for executor loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for executor module directory, support for executor loading disabled.\n");
 }
 executors_dir = (char *) module_type_value->value;

 module_type_directive = CfgSection_get_directive(modules_section, "Connector", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No connector module directory defined, support for connector loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for connector module directory, support for connector loading disabled.\n");
 }
 connectors_dir = (char *) module_type_value->value;
 
 module_type_directive = CfgSection_get_directive(modules_section, "Communicator", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No communicator module directory defined, support for communicator loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for communicator module directory, support for communicator loading disabled.\n");
 }
 communicators_dir = (char *) module_type_value->value;

 module_type_directive = CfgSection_get_directive(modules_section, "Auth", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No authentication / autherization module directory defined, support for autentication / autherization loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for authentication / autherization  module directory, support for authentication / autherization loading disabled.\n");
 }
 auth_dir = (char *) module_type_value->value;

 module_type_directive = CfgSection_get_directive(modules_section, "Protocol", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No protocol module directory defined, support for protocol loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for protocol module directory, support for protocol loading disabled.\n");
 }
 protocols_dir = (char *) module_type_value->value;
 
 return SUCCESS;
}

Module *modman_load_module(char *name, int type) {
 Module *mod = Module_create(name, type);
 
 mod->file = _get_module_path(type, strlen(name) + 4);
 
 if (mod->file != NULL) {
  strcat(mod->file, "/");
  strcat(mod->file, name);
  strcat(mod->file, ".so");
  mod->handle = dlopen(mod->file, RTLD_LAZY);
  if (mod->handle != NULL) {
   mod->status = MODULE_STATUS_LOADED;
   char *version = (char *) dlsym(ret, "opendaemon_version");
   if (strcmp(version, OPENDAEMON_VERSION) != 0) {
    printf("\t* Unable to load module \"%s\": Incompaitbile OpenDaemon version.", mod->file);
    free(mod->file);
    dlclose(mod->handle);
    Module_free(mod);
    mod = NULL;
   }
   else {
    mod->get_instance_func = dlsym(ret, "_get_module_instance");
    if (mod->get_instance_func == NULL) {
     printf("\t* Unable to load module \"%s\": Cannot find module initialization routine.", mod->file);
     free(mod->file);
     dlclose(mod->handle);
     Module_free(mod);
     mod = NULL;
    }

    mod->destroy_instance_func = dlsym(ret, "_destroy_module_instance");
    if (mod->destroy_instance_func == NULL) {
     printf("\t* Unable to load module \"%s\": Cannot find module termination routine.", mod->file);
     free(mod->file);
     dlclose(mod->handle);
     Module_free(mod);
     mod = NULL;
    }
   }
  }
  else {
   free(mod->file);
   Module_free(mod);
   mod = NULL;
  }
 }
 else {
  Module_free(mod);
  mod = NULL;
 }

 return mod;
}

void *modman_get_module_instance(Module *mod, char *config_file) {
 void *ret = NULL;
 char *config_path = NULL;

 if (config_file != NULL) {
  config_path = _get_module_path(mod->type, strlen(config_file) + 1);
  if (config_path != NULL) {
   strcat(config_path, "/");
   strcat(config_path, config_file);
  }
 }
 else {
  config_path = _get_module_path(mod->type, strlen(mod->name) + 6);
  if (config_path != NULL) {
   strcat(config_path, "/");
   strcat(config_path, mod->name);
   strcat(config_path, ".conf");
  }
 }
 
 CfgSection *mod_cfg;
 if (config_path != NULL) {
  mod_cfg = cfg_create_tree(config_path);
  free(config_path);
 }
 else {
  mod_cfg = NULL;
 }
 ret = (*mod->get_instance_func) (mod_cfg); 
 
 return ret;
}

void modman_destroy_module_instance(Module *mod, void *instance) {
 (*mod->destroy_instance_func) (instance);
}

Status modman_unload_module(Module *mod) {
 if (dlclose(mod->handle) == 0) {
  if (mod->file != NULL) {
   free(mod->file);
  }
  Module_free(mod)
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

char *_get_module_path(int type, int add_len) {
 char *ret = NULL;
 
 if (type == MODULE_INTERFACE_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(interfaces_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, interfaces_dir);
 }
 else if (type == MODULE_ENGINE_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(data_stores_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, data_stores_dir);
 }
 else if (type == MODULE_PROCESSOR_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(processors_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, processors_dir);
 }
 else if (type == MODULE_EXECUTOR_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(executors_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, executors_dir);
 }
 else if (type == MODULE_COMMUNICATOR_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(communicators_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, communicators_dir);
 }
 else if (type == MODULE_CONNECTOR_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(connectors_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, connectors_dir);
 }
 else if (type == MODULE_PROTOCOL_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(protocols_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, protocols_dir);
 }
 else if (type == MODULE_AUTH_MODULE) {
  ret = (char *) malloc((strlen(server_root) + strlen(auth_dir) + add_len + 1) * sizeof(char));
  strcpy(ret, server_root);
  strcat(ret, auth_dir);
 }

 return ret;
}
