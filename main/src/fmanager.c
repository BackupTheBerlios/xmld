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
#include <string.h>
#include "xmld_types.h"
#include "engine_xmld.h"

/* FIXME: file config should be checked to decide the engine to be used.
 * Currently we use only one engine defined in engine_xmld.h
 * a variable of type struct XMLDEngine called `engine_xmld' carries
 * this only one engine 
 */

short fmanager_handle(struct XMLDWork *work) {
 work->res=(struct XMLDResource *) malloc(sizeof(struct XMLDResource));
 work->res->engine=&engine_xmld;
 work->res->data_source=(*(work->res->engine->init)) (work);
 if (work->res->data_source==0) {
  return -1;
 }
 else {
  return 0;
 }
}
