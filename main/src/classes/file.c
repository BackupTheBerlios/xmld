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
#include "cfg.h"

/*
 * Creates a new file structure.
 * name: The name of the file.
 * returns: The newly created file structure.
 */
File *File_create(void) {
 File *file=(File *) malloc(sizeof(File));
 file->file=NULL;
 file->data=NULL;
 return file;
}

/*
 * Frees a file structure.
 */
void File_free(File *file) {
 if (file != NULL) {
  Assoc_free(file->data);
  free(file);
 }
}
