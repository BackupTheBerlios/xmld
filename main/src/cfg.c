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
#include "cfg_parser.h"
#include "cfg.h"

/*
 * : Initiates the configuration manager.
 * returns: whether successful.
 */
short cfg_init() {
 cfg_parser_parse();
 return 1;
}

/*
 * : Gets the value of a particular configuration
 * directive.
 * key: the name of the configuration directive
 * whose value is to be aquired.
 * returns: a void pointer to the value of the given
 * directive or NULL if the directive was not found
 * or if an error occured during retrieval.
 */ 
void *cfg_get(char *key) {
 return (void *) NULL;
}

/*
 * : Gets the engine which is to be used
 * in processing a given file.
 * file: the name of the file for which
 * an engine name is to be returned.
 * returns: if the file had an engine name
 * associated to it in configurations
 * the name of that engine is returned, and
 * if not, NULL is returned (MIME checking
 * should be used to determine the engine).
 */
char *cfg_get_engine(char *file) {
 return (char *) NULL;
}

/*
 * : Associates a particular file name to
 * a particular engine.
 * file: the file to be associated.
 * engine: the engine to be linked to the file.
 */ 
void cfg_set_engine(char *file, char *engine) { 
}

/*
 * : Gets the default engine for the given _mime_
 * returns: the name of the mentioned engine.
 */
char *cfg_get_engine_mime(char *mime) { 
 return (char *) NULL;
}

/*
 * : Sets the default engine for a given _mime_.
 */
void cfg_set_engine_mime(char *mime, char *engine) {
}

/*
 * Finalizes and cleans up the parse tree of the conf.
 * returns: whether successful.
 */
short cfg_shutdown() {
 cfg_parser_clean();
 return 1;
}
 
