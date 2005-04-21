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
XMLDInterface *XMLDInterface_create(char *name) {
 XMLDInterface *interface=(XMLDInterface *) malloc(sizeof(XMLDInterface));
 interface->name = name;
 return interface;
}

/*
 * : Copies the src interface to the dest interface.
 */
void XMLDInterface_copy(XMLDInterface *src, XMLDInterface *dest) {
 dest->name = src->name;
 dest->port = src->port;
 dest->init = src->init;
 dest->destroy = src->destroy;
 dest->prepare_conn = src->prepare_conn;
 dest->cleanup_conn = src->cleanup_conn;
 dest->prepare = src->prepare;
 dest->cleanup = src->cleanup;
 dest->parse = src->parse;
 dest->walk = src->walk;
 dest->get_response = src->get_response;
}

/*
 * : Frees a created interface structure.
 * req: the interface structure to be freed.
 */
void XMLDInterface_free(XMLDInterface *interface) {
 if (interface != NULL) {
  XMLDInterface_free_content((void *) interface);
  free(interface);
 }  
}

/*
 * : Frees the memory internally allocated by an interface structure.
 * req: the structure whose internally memory is to be freed.
 */
void XMLDInterface_free_content(void *interface) {
}
