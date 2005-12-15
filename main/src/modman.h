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
char *parsers_dir;

/* Type flags of modman_load_module */
#define MODMAN_ENGINE_MODULE    0
#define MODMAN_INTERFACE_MODULE 1
#define MODMAN_PROCESSOR_MODULE 2
#define MODMAN_PARSER_MODULE    3

Status modman_init(void);
Status modman_load_module(char *, int);
Status modman_unload_module(void *);

#endif /* __MODMAN_H */
