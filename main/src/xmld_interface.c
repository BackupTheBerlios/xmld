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

/*
 * : Creates a new list of interface structures.
 * returns: the newly created list.
 */
XMLDInterfaceList *XMLDInterfaceList_create() {
 XMLDInterfaceList *list=XMLDList_create(sizeof(XMLDInterface), XMLDInterface_free_content);
 return list;
}

/*
 * : Adds a new element to an already created list of interface
 * structures.
 * returns: a pointer to the newly added element.
 */
XMLDInterface *XMLDInterfaceList_add(XMLDInterfaceList *list, char *name) {
 XMLDInterface *interface=(XMLDInterface *) XMLDList_add(list);
 interface->name = name;
 return interface;
}

/*
 * : Searches a list of interface structures for an interface that
 * has a particular name.
 * list: the list to search.
 * name: the name to look for.
 * returns: a pointer to the interface that has the given name or NULL
 * if not found. If there was more than one interface having the same
 * name, a pointer to the first one is returned.
 */
XMLDInterface *XMLDInterfaceList_search_by_name(XMLDInterfaceList *list, char *name) {
 XMLDList_reset(list);
 XMLDInterface *interface=NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDInterface *) XMLDList_curr(list))->name, name)==0) {
   interface = (XMLDInterface *) XMLDList_curr(list);
   break;
  }
 }
 return interface;
}

/*
 * : Searches a list of interface structures for an interface that
 * has a particular port.
 * list: the list to search.
 * port: the port to look for.
 * returns: a pointer to the interface that has the given port or NULL
 * if not found. If there was more than one interface having the same
 * port, a pointer to the first one is returned.
 */
XMLDInterface *XMLDInterfaceList_search_by_port(XMLDInterfaceList *list, int port) {
 XMLDList_reset(list);
 XMLDInterface *interface=NULL;
 while (XMLDList_next(list)) {
  if (((XMLDInterface *) XMLDList_curr(list))->port == port) {
   interface = (XMLDInterface *) XMLDList_curr(list);
   break;
  }
 }
 return interface;
}
