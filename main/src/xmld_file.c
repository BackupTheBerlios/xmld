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
XMLDFile *XMLDFile_create(void) {
 XMLDFile *file=(XMLDFile *) malloc(sizeof(XMLDFile));
 file->file=NULL;
 file->data=NULL;
 return file;
}

/*
 * Frees a file structure.
 */
void XMLDFile_free(XMLDFile *file) {
 if (file != NULL) {
  XMLDAssoc_free(file->data);
  free(file);
 }
}
