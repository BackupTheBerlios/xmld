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
 
#ifndef __MODULE_H
#define __MODULE_H

/* Module type flags */
#define MODULE_ENGINE_MODULE    0
#define MODULE_INTERFACE_MODULE 1
#define MODULE_PROCESSOR_MODULE 2
#define MODULE_EXECUTOR_MODULE  4
#define MODULE_PROTOCOL_MODULE  5
#define MODULE_CONNECTOR_MODULE 6
#define MODULE_COMMUNICATOR_MODULE 7
#define MODULE_AUTH_MODULE 8

/* Module status flags */
#define MODULE_STATUS_LOADED 1
#define MODULE_STATUS_NOT_LOADED 2

struct Module {
 int type;
 int status;
 void *handle;
 char *file;
 char *name;
 void *(*get_instance_func) (CfgSection *);
 void (*destroy_instance_func) (void *);
 CfgSection *(*get_instance_conf_func) (void *);
};

typedef struct Module Module;

Module *Module_create(char *, int);
void Module_free(Module *);

#endif /* __MODULE_H */
