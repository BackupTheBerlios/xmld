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

/*
 * : Creates a new interface structure.
 * returns: the newly created interface structure.
 */
Interface *Interface_create() {
 Interface *interface=(Interface *) malloc(sizeof(Interface));
 interface->data = NULL;
 return interface;
}

/*
 * : Copies the src interface to the dest interface.
 */
void Interface_copy(Interface *src, Interface *dest) {
 dest->port = src->port;
 dest->init = src->init;
 dest->destroy = src->destroy;
 dest->user_connection = src->user_connection;
 dest->data = src->data;
}

/*
 * : Frees a created interface structure.
 * req: the interface structure to be freed.
 */
void Interface_free(Interface *interface) {
 if (interface != NULL)  {
  Assoc_free(interface->data);
  free(interface);
 }  
}
