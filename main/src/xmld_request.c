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
#include "xmlddef.h"
#include "mutils.h"
#include "xmld_list.h"
#include "xmld_request.h"

/*
 * : Creates a new request structure.
 * returns: the newly created request structure.
 */
XMLDRequest *XMLDRequest_create() {
 XMLDRequest *req=(XMLDRequest *) malloc(sizeof(XMLDRequest));
 req->file=NULL;
 req->retr=NULL;
 req->vals=NULL;
 req->where=NULL;
 req->misc=NULL;
 return req;
}

/*
 * : Frees a created request structure.
 * req: the request structure to be freed.
 */
void XMLDRequest_free(XMLDRequest *req) {
 if (req != NULL) {
  XMLDRequest_free_content((void *)req);
  free(req);
 }  
}

/*
 * : Frees the memory internally allocated by a request structure.
 * req: the structure whose internally memory is to be freed.
 */
void XMLDRequest_free_content(void *req) {
 cfree(((XMLDRequest *) req)->file);
 XMLDList_free(((XMLDRequest *) req)->retr);
 XMLDList_free(((XMLDRequest *) req)->vals);
 XMLDList_free(((XMLDRequest *) req)->where);
 XMLDList_free(((XMLDRequest *) req)->misc);
}

/*
 * : Creates a new list of request structures.
 * returns: the newly created list.
 */
XMLDList *XMLDRequest_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDRequest), XMLDRequest_free_content);
 return list;
}

/*
 * : Adds a new element to an already created list of request
 * structures.
 * returns: a pointer to the newly added element.
 */
XMLDRequest *XMLDRequest_add_to_list(XMLDList *list) {
 XMLDRequest *req=(XMLDRequest *) XMLDList_add(list);
 req->file=NULL;
 req->retr=NULL;
 req->vals=NULL;
 req->where=NULL;
 req->misc=NULL;
 return req;
}
