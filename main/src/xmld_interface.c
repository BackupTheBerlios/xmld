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
 interface->name = name
 return interface;
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
 free(((XMLDInterface *) interface)->name);
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
 return req;
}
