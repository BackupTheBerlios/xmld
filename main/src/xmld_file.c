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
XMLDFile *XMLDFile_create(char *name) {
 XMLDFile *file=(XMLDFile *) malloc(sizeof(XMLDFile));
 if (name != NULL) {
  file->name=(char *) malloc((strlen(name)+1)*sizeof(char));
  strcpy(file->name, name);
 }
 file->data=NULL;
 file->store=NULL;
 file->engine=NULL;
 file->level=0;
 file->priv=9;
 return file;
}

/*
 * Frees a file structure.
 */
void XMLDFile_free(XMLDFile *file) {
 if (file != NULL) {
  XMLDFile_free_content((void *) file);
  free(file);
 }
}

/*
 * Frees the memory internally allocated by a file structure.
 */
void XMLDFile_free_content(void *file) {
 if (((XMLDFile *) file)->name != NULL) {
  free(((XMLDFile *) file)->name);
 }
}
