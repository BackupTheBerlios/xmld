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

 module_type_directive = CfgSection_get_directive(modules_section, "Parser", 0);
 if (module_type_directive == NULL) {
  printf("\t* modman: No parser module directory defined, support for parser loading disabled.\n");
 }
 if (module_type_value->type != CFG_STRING) {
  printf("\t* modman: Bad configuration value for parser module directory, support for parser loading disabled.\n");
 }
 parsers_dir = (char *) module_type_value->value;

 return SUCCESS;
}

void *modman_load_module(char *name, int type) {
 void *ret = NULL;
 char *file = NULL;
 
 if (type == MODMAN_INTERFACE_MODULE) {
  file = (char *) malloc((strlen(server_root) + strlen(interfaces_dir) + strlen(name) + 5) * sizeof(char));
  strcpy(file, server_root);
  strcat(file, interfaces_dir);
 }
 else if (type == MODMAN_ENGINE_MODULE) {
  file = (char *) malloc((strlen(server_root) + strlen(engines_dir) + strlen(name) + 5) * sizeof(char));
  strcpy(file, server_root);
  strcat(file, engines_dir);
 }
 else if (type == MODMAN_PROCESSOR_MODULE) {
  file = (char *) malloc((strlen(server_root) + strlen(processors_dir) + strlen(name) + 5) * sizeof(char));
  strcpy(file, server_root);
  strcat(file, processors_dir);
 }
 else if (type == MODMAN_PARSER_MODULE) {
  file = (char *) malloc((strlen(server_root) + strlen(parsers_dir) + strlen(name) + 5) * sizeof(char));
  strcpy(file, server_root);
  strcat(file, parsers_dir);
 }
 else {
  return NULL;
 }
 
 strcat(file, "/");
 strcat(file, name);
 strcat(file, ".so");
 ret = dlopen(file, RTLD_LAZY);
 if (ret) {
  char *version = (char *) dlsym(ret, "opendaemon_version");
  if (strcmp(version, OPENDAEMON_VERSION) != 0) {
   printf("\t* Unable to load module \"%s\": Incompaitbile OpenDaemon version.", file);
   dlclose(ret);
   ret = NULL;
  }
 }
 free(file);
 return ret;
}

Status modman_unload_module(void *handle) {
 if (dlclose(handle) == 0) {
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}
