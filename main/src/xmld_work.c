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
 work->conn=NULL;
 work->interface=NULL;
 work->store=NULL;
 return work;
}

/*
 * : Frees an allocated work structure.
 * work: the work structure to free.
 */
void XMLDWork_free(XMLDWork *work) {
 if (work != NULL) {
  XMLDWork_free_content((void *) work);
  free(work);
 } 
}

/*
 * : Frees the memory internally allocated by a work structure.
 * work: the work structure whose memory is to be freed.
 */
void XMLDWork_free_content(void *work) {
 XMLDList_free(((XMLDWork *) work)->files);
 XMLDConnection_free(((XMLDWork *) work)->conn);
}

/*
 * : Creates a list of work structures.
 * returns: the newly created list.
 */
XMLDWorkList *XMLDWorkList_create() {
 XMLDWorkList *list=XMLDList_create(sizeof(XMLDWork), XMLDWork_free_content);
 return list;
}

/*
 * : Adds an element to a created list of work structures.
 * list: the list to which the new element is to be added.
 * req, resp, conn, res: see XMLDWork_create
 * returns: a pointer to the newly added element.
 */
XMLDWork *XMLDWorkList_add(XMLDWorkList *list) {
 XMLDWork *work=(XMLDWork *) XMLDList_add(list);
 work->files=NULL;
 work->conn=NULL;
 work->interface=NULL;
 work->store=NULL;
 return work;
}
