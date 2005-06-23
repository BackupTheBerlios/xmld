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
 * : Creates a new work structure.
 * returns: the newly created work structure.
 */
XMLDWork *XMLDWork_create() {
 XMLDWork *work=(XMLDWork *) malloc(sizeof(XMLDWork));
 work->files=NULL;
 work->interface=NULL;
 work->data=NULL;
 return work;
}

/*
 * : Frees an allocated work structure.
 * work: the work structure to free.
 */
void XMLDWork_free(XMLDWork *work) {
 if (work != NULL) {
  XMLDAssocWalker walker;
  walker.subject = work->files;
  walker.curr_index = -1;
  while (XMLDAssocWalker_next(&walker)) {
   XMLDFile_free((XMLDFile *) XMLDAssocWalker_get_current_data(&walker));
  }
  XMLDAssoc_free(work->files);
  free(work);
 } 
}
