#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "xmld_types.h"
#include "engine_xmld.h"

/* FIXME: file config should be checked to decide the engine to be used.
 * Currently we use only one engine defined in engine_xmld.h
 * a variable of type struct XMLDEngine called `engine' carries
 * this only one engine 
 */

xmld_status_t fmanager_handle(struct XMLDWork *work) {
 work->res=(struct XMLDResource *) malloc(sizeof(struct XMLDResource));
 work->res->engine=&engine_xmld;
 work->res->data_source=(*(work->res->engine->init)) (work); /* must descide mode according to XMLDRequest->type */
 if (work->res->data_source==0) {
  return XMLD_FAILURE;
 }
 else {
  return XMLD_SUCCESS;
 }
}
