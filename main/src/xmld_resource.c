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
#include "xmlddef.h"
#include "mutils.h"
#include "xmld_list.h"
#include "xmld_col.h"
#include "xmld_row.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_aggr_table.h"
#include "xmld_cond.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_resource.h"
#include "xmld_response.h"
#include "xmld_request.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"

/*
 * : Creates a new resource structure.
 * returns: the newly created resource structure.
 */
XMLDResource *XMLDResource_create() {
 XMLDResource *res=(XMLDResource *) malloc(sizeof(XMLDResource));
 res->engine=NULL;
 res->data_source=NULL;
 res->store=NULL;
 return res;
}

/*
 * : Frees an allocated resource structure.
 * res: the resource structure to free.
 */
void XMLDResource_free(XMLDResource *res) {
 if (res != NULL) {
  XMLDResource_free_content((void *) res);
  free(res);
 } 
}

/*
 * : Frees the memory internally allocated by a resource structure.
 * res: the resource structure whose internal memory is to be freed.
 */
void XMLDResource_free_content(void *res) {
/*
 * Freeing data items of this structure is an engine specific task.
 */ 
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
XMLDResource *XMLDResource_add_to_list(XMLDList *list) {
 XMLDResource *res=(XMLDResource *) XMLDList_add(list);
 res->engine=NULL;
 res->data_source=NULL;
 res->store=NULL;
 return res;
}
