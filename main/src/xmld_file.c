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
 cfree(((XMLDFile *) file)->name);
}

/*
 * Creates a list of files.
 */
XMLDFileList *XMLDFileList_create() {
 return XMLDList_create(sizeof(XMLDFile), XMLDFile_free_content);
}

/*
 * Adds a new element to a list of files.
 * returns: a pointer to the newly added element.
 */
XMLDFile *XMLDFileList_add(XMLDFileList *list, char *name) {
 XMLDFile *file=(XMLDFile *) XMLDList_add(list);
 if (name != NULL) {
  file->name=(char *) malloc((strlen(name)+1)*sizeof(char));
  strcpy(file->name, name);
 }
 file->data=NULL;
 file->store=NULL;
 file->engine=NULL;
 file->level=0;
 file->priv=0;
 return file;
}

/*
 * Searches a list of files for a file with a particular name.
 */
XMLDFile *XMLDFileList_search_by_name(XMLDFileList *list, char *name) {
 XMLDFile *file=NULL;
 XMLDList_reset(list);
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDFile *) XMLDList_curr(list))->name, name) == 0) {
   file = (XMLDFile *) XMLDList_curr(list);
   break;
  }
 }
 return file;
}

/*
 * Gets the maximum level field in a list of files.
 */
int XMLDFileList_get_max_level(XMLDFileList *list) {
 int ret=0;
 XMLDList_reset(list);
 while (XMLDList_next(list)) {
  if (((XMLDFile *) XMLDList_curr(list))->level > ret) {
   ret = ((XMLDFile *) XMLDList_curr(list))->level;
  }
 }
 return ret;
}






