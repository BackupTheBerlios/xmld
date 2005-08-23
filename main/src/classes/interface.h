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

#ifndef __INTERFACE_H
#define __INTERFACE_H

/* Represents a user interface */
struct Interface {
 int port;
 char *error;
 Status (*init) (Interface *, CfgDirective *);
 void (*destroy) (Interface *);
 void (*user_connection) (void *);
 char *(*get_error_message) (Interface *);
 Assoc *data;
};

#ifndef INTERFACE_TYPE_DEFINED
#define INTERFACE_TYPE_DEFINED
 typedef struct Interface Interface;
#endif /* INTERFACE_TYPE_DEFINED */

Interface *Interface_create(void);
void Interface_copy(Interface *, Interface *);
void Interface_free(Interface *);

#endif /* __INTERFACE_H */
