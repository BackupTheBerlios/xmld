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
 * : Creates a new engine structure.
 * name: the name of the new engine. (not copied)
 * returns: the newly created engine.
 */
XMLDEngine *XMLDEngine_create(char *name) {
 XMLDEngine *engine=(XMLDEngine *) malloc(sizeof(XMLDEngine));
 engine->name = name;
 return engine;
}

/*
 * : Frees a created engine structure.
 * engine: the engine to free.
 */
void XMLDEngine_free(XMLDEngine *engine) {
 if (engine != NULL) {
  XMLDEngine_free_content((void*) engine);
  free(engine);
 } 
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
XMLDEngineList *XMLDEngineList_create() {
 return XMLDList_create(sizeof(XMLDEngine), XMLDEngine_free_content);
}

/*
 * : Adds a new element to a list of engine structures.
 * list: the list to which the new engine structure will be added.
 * name: the name of the newly added engine structure.
 * returns: a pointer to the newly added element.
 */
XMLDEngine *XMLDEngineList_add(XMLDEngineList *list, char *name) {
 XMLDEngine *engine=(XMLDEngine *) XMLDList_add(list);
 engine->name = name;
 return engine;
}

/*
 * : Searches a list of engine structures for an engine that
 * has a particular name.
 * list: the list to search.
 * name: the name to look for.
 * returns: a pointer to the engine that has the given name or NULL
 * if not found. If there was more than one engine having the same
 * name, a pointer to the first one is returned.
 */
XMLDEngine *XMLDEngineList_search_by_name(XMLDEngineList *list, char *name) {
 XMLDList_reset(list);
 XMLDEngine *engine=NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDEngine *) XMLDList_curr(list))->name, name)==0) {
   engine = (XMLDEngine *) XMLDList_curr(list);
   break;
  }
 }
 return engine;
}
