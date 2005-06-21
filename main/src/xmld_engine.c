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
XMLDEngine *XMLDEngine_create() {
 XMLDEngine *engine=(XMLDEngine *) malloc(sizeof(XMLDEngine));
 return engine;
}

/*
 * : Frees a created engine structure.
 * engine: the engine to free.
 */
void XMLDEngine_free(XMLDEngine *engine) {
 if (engine != NULL) {
  free(engine);
 } 
}
