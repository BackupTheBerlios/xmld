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

#ifndef __MODMAN_H
#define __MODMAN_H

char *interfaces_dir;
char *engines_dir;
char *processors_dir;
char *executors_dir;
char *communicators_dir;
char *connectors_dir;
char *auth_dir;
char *protocols_dir;

Status modman_init(void);
Module *modman_load_module(char *, int);
void *modman_get_module_instance(Module *, char *);
Status modman_unload_module(Module *);
void modman_destroy_module_instance(Module *, void *);
char *_get_module_path(int, int);

#endif /* __MODMAN_H */
