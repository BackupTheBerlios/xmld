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
 
#include "interface.h"

Interface *Interface_create(void) {
 Interface *if_inst = (Interface *) malloc(sizeof(Interface));
 if_inst->main = inf_inst->init = if_inst->msg = NULL;
 return if_inst;
}

void Interface_free(Interface *if_inst) {
 free(if_inst);
}
