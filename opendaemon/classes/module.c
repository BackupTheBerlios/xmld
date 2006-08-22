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

#include "../includes.h"
#include "module.h"

Module *Module_create(char *name, int type) {
 Module *mod = (Module *) malloc(sizeof(Module));
 mod->type = type;
 mod->file = NULL;
 mod->status = MODULE_STATUS_NOT_LOADED;
 mod->handle = NULL;
 mod->name = name;
 mod->get_instance_func = NULL;
 return mod;
}

void Module_free(Module *mod) {
 free(mod);
}
