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
#include "mutils.h"
#include <string.h>
#include "cfg.h"
#include "xmld_list.h"
struct XMLDFunc;
#ifndef XMLD_FUNC_TYPE_DEFINED
#define XMLD_FUNC_TYPE_DEFINED
 typedef struct XMLDFunc XMLDFunc;
#endif /* XMLD_FUNC_TYPE_DEFINED */
#include "xmld_expr.h"
#include "xmld_func.h"
#include "xmld_cond.h"
#include "xmld_col.h"
#include "xmld_row.h"
#include "xmld_aggr_table.h"
struct XMLDEngine;
#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
#include "xmld_request.h"
#include "xmld_response.h"
#include "xmld_resource.h"
#include "xmld_connection.h"
#include "xmld_work.h"
#include "xmld_engine.h"

/*
 * : Creates a new work structure.
 * returns: the newly created work structure.
 */
XMLDWork *XMLDWork_create() {
 XMLDWork *work=(XMLDWork *) malloc(sizeof(XMLDWork));
 work->req=NULL;
 work->resp=NULL;
 work->res=NULL;
 work->conn=NULL;
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
 XMLDRequest_free(((XMLDWork *) work)->req);
 XMLDResource_free(((XMLDWork *) work)->res);
 XMLDConnection_free(((XMLDWork *) work)->conn);
 XMLDResponse_free(((XMLDWork *) work)->resp); 
}

/*
 * : Returns the full file name of the request structure
 * associated with the given work structure concatenated to
 * the current directory of the connection structure associated
 * to the given work structure.
 * work: The work structure mentioned.
 * returns: The full file location from the root of the given
 * work structure.
 */
char *XMLDWork_get_full_file(XMLDWork *work) {
 char *full_file;
 if (*(work->req->file) == '/') {
  full_file=(char *) malloc((strlen(work->req->file)+strlen(document_root)+1)*sizeof(char));
  strcpy(full_file, document_root);
  strcat(full_file, work->req->file);
  return full_file;
 }

 full_file=(char *) malloc(strlen(document_root)+strlen((work->conn->curr_dir)+strlen(work->req->file)+1)*sizeof(char));
 strcpy(full_file, document_root);
 strcat(full_file, work->conn->curr_dir);
 strcat(full_file, work->req->file);
 return full_file;
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
XMLDWork *XMLDWork_add_to_list(XMLDList *list) {
 XMLDWork *work=(XMLDWork *) XMLDList_add(list);
 work->req=NULL;
 work->resp=NULL;
 work->res=NULL;
 work->conn=NULL;
 return work;
}
