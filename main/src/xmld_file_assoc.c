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
#include <stdio.h>
#include <string.h>
#include "xmld_list.h"
#include "xmld_file_assoc.h"

/*
 * : Creates a new file association structure.
 * file_name: The name of the file to be associated.
 * fd: The file descriptor associated to the given file
 * name.
 * returns: The newly created file association structure.
 */
XMLDFileAssoc *XMLDFileAssoc_create(char *file_name, FILE *fd) {
 XMLDFileAssoc *fassoc=(XMLDFileAssoc *) malloc(sizeof(XMLDFileAssoc));
 if (file_name != NULL) {
  fassoc->file_name=(char *) malloc((strlen(file_name)+1)*sizeof(char));
  strcpy(fassoc->file_name, file_name);
 }
 else {
  fassoc->file_name=NULL;
 }
 fassoc->fd=fd;
 return fassoc;
}

/*
 * : Frees a file association structure.
 * fassoc: The file association structure to free.
 */
void XMLDFileAssoc_free(XMLDFileAssoc *fassoc) {
 XMLDFileAssoc_free_content((void *) fassoc);
 free(fassoc);
}

/*
 * : Frees the memory internally used by a file
 * association structure.
 * fassoc: The named file association structure. 
 */
void XMLDFileAssoc_free_content(void *fassoc) {
 if (((XMLDFileAssoc *) fassoc)->file_name != NULL) {
  free(((XMLDFileAssoc *) fassoc)->file_name);
 }
}

/*
 * : Creates a list of file association structures.
 * returns: The newly created list.
 */
XMLDList *XMLFileAssoc_create_list() {
 return XMLDList_create(sizeof(XMLDFileAssoc), XMLDFileAssoc_free_content);
}

/*
 * : Adds a new element to a list of file association structures.
 * file_name, fd: observe creation function.
 * returns: A pointer to the newly added element.
 */
XMLDFileAssoc *XMLDFileAssoc_add_to_list(XMLDList *list, char *file_name, FILE *fd) {
 XMLDFileAssoc *fassoc=(XMLDFileAssoc *) XMLDList_add(list);
 if (file_name != NULL) {
  fassoc->file_name=(char *) malloc((strlen(file_name)+1)*sizeof(char));
  strcpy(fassoc->file_name, file_name);
 }
 else {
  fassoc->file_name=NULL;
 }
 fassoc->fd=fd;
 return fassoc;
}

/*
 * : Searches a list of file association structures for one
 * that has a particular file_name.
 * returns: A pointer to the fassoc having the given file_name
 * or NULL if it wasn't found.
 */
XMLDFileAssoc *XMLDFileAssoc_search_list_by_file_name(XMLDList *list, char *file_name) {
 XMLDList_reset(list);
 XMLDFileAssoc *fassoc=NULL;
 while (XMLDList_next(list)) {
  if (strcmp(((XMLDFileAssoc *) XMLDList_curr(list))->file_name, file_name) == 0) {
   fassoc = (XMLDFileAssoc *) XMLDList_curr(list);
   break;
  }
 }
 return fassoc;
}
