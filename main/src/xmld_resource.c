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

#include <stdlib.h>
#include "xmld_list.h"
#include "xmld_engine.h"
#include "xmld_resource.h"

/*
 * : Creates a new resource structure.
 * engine (optional): the engine that serves the request.
 * data_source (optional): the source of data on which the
 * engine will work.
 * returns: the newly created resource structure.
 */
XMLDResource *XMLDResource_create(XMLDEngine *engine, void *data_source) {
 XMLDResource *res=(XMLDResource *) malloc(sizeof(XMLDResource));
 res->engine=engine;
 res->data_source=data_source;
 return res;
}

/*
 * : Frees an allocated resource structure.
 * res: the resource structure to free.
 */
void XMLDResource_free(XMLDResource *res) {
 XMLDResource_free_content((void *) res);
 free(res);
}

/*
 * : Frees the memory internally allocated by a resource structure.
 * res: the resource structure whose internal memory is to be freed.
 */
void XMLDResource_free_content(void *res) {
 /*
  * Note: data_source isn't freed, should be freed with destroy() of 
  * the engine, which must have been called before freeing the engine
  * itself (unless init() of the engine wasn't called).
  */ 
 XMLDEngine_free(((XMLDResource *) res)->engine);
}

/*
 * : Creates a list of resource structures.
 * returns: the newly created list.
 */
XMLDList *XMLDResource_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDResource), XMLDResource_free_content);
 return list;
}

/*
 * : Adds a new element to a list of resource structures.
 * engine, data_source: see XMLDResource_create
 * list: the list to which the new element is to be added.
 * returns: a pointer to the newly added element.
 */
XMLDResource *XMLDResource_add_to_list(XMLDList *list, XMLDEngine *engine, void *data_source) {
 XMLDResource *res=(XMLDResource *) XMLDList_add(list);
 res->engine=engine;
 res->data_source=data_source;
 return res;
}
