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
#include "xmld_list.h"
#include "xmld_response.h"
#include "xmld_resource.h"
#include "xmld_connection.h"
#include "xmld_work.h"

/*
 * : Creates a new work structure.
 * req (optional): an initial request structure.
 * resp (optional): an initial response structure.
 * res (optional): an initial resource structure.
 * conn (optional): an initial connection structure.
 * returns: the newly created work structure.
 */
XMLDWork *XMLDWork_create(XMLDRequest *req, XMLDResponse *resp, XMLDConnection *conn, XMLDResource *res) {
 XMLDWork *work=(XMLDWork *) malloc(sizeof(XMLDWork));
 work->req=req;
 work->resp=resp;
 work->conn=conn;
 work->res=res;
 return work;
}

/*
 * : Frees an allocated work structure.
 * work: the work structure to free.
 */
void XMLDWork_free(XMLDWork *work) {
 XMLDWork_free_content((void *) work);
 free(work);
}

/*
 * : Frees the memory internally allocated by a work structure.
 * work: the work structure whose memory is to be freed.
 */
void XMLDWork_free_content(void *work) {
 XMLDRequest_free(((XMLDWork *) work)->req);
 XMLDResponse_free(((XMLDWork *) work)->resp);
 XMLDConnection_free(((XMLDWork *) work)->conn);
 XMLDResource_free(((XMLDWork *) work)->res);
}

/*
 * : Creates a list of work structures.
 * returns: the newly created list.
 */
XMLDList *XMLDWork_create_list() {
 XMLDList *list=XMLDList_create(sizeof(XMLDWork), XMLDWork_free_content);
 return list;
}

/*
 * : Adds an element to a created list of work structures.
 * list: the list to which the new element is to be added.
 * req, resp, conn, res: see XMLDWork_create
 * returns: a pointer to the newly added element.
 */
XMLDWork *XMLDWork_add_to_list(XMLDList *list, XMLDRequest *req, XMLDResponse *resp, XMLDConnection *conn, XMLDResource *res) {
 XMLDWork *work=(XMLDWork *) XMLDList_add(list);
 work->req=req;
 work->resp=resp;
 work->conn=conn;
 work->res=res;
 return work;
}
