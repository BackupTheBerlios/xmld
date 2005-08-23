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

#include "../includes.h"

/*
 * : Creates a new engine structure.
 * name: the name of the new engine. (not copied)
 * returns: the newly created engine.
 */
Engine *Engine_create() {
 Engine *engine=(Engine *) malloc(sizeof(Engine));
 return engine;
}

/*
 * : Frees a created engine structure.
 * engine: the engine to free.
 */
void Engine_free(Engine *engine) {
 if (engine != NULL) {
  free(engine);
 } 
}
