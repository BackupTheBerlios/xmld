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
#include "xmld_func.h"
#include "xmld_expr.h"
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
 * : Creates a new engine structure.
 * returns: the newly created engine.
 */
XMLDEngine *XMLDEngine_create() {
 return ((XMLDEngine *) malloc(sizeof(XMLDEngine)));
}

/*
 * : Frees a created engine structure.
 * engine: the engine to free.
 */
void XMLDEngine_free(XMLDEngine *engine) {
 XMLDEngine_free_content((void*) engine);
 free(engine);
}

/*
 * : Frees the memory internally allocated by an engine structure.
 * engine: the engine whose memory is to be freed.
 */
void XMLDEngine_free_content(void *engine) { 
}

/*
 * : Creates a list of engine structures.
 * returns: the newly created list.
 */
XMLDList *XMLDEngine_create_list() {
 return XMLDList_create(sizeof(XMLDEngine), XMLDEngine_free_content);
}

/*
 * : Adds a new element to a list of engine structures.
 * list: the list to which the new engine structure will be added.
 * returns: a pointer to the newly added element.
 */
XMLDEngine *XMLDEngine_add_to_list(XMLDList *list) {
 return ((XMLDEngine *) XMLDList_add(list));
}

