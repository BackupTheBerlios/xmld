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

#include "../includes.h"
#include "xmldsql.h"

void xmldsql_init(void) {
 /*
  * For global initialization of the interface.
  */
}

void xmldsql_destroy(void) {
 /*
  * For global destruction of the interface.
  */ 
}

XMLDStatus xmldsql_prepare_conn(XMLDWork *work) {
 return XMLD_SUCCESS;
}

void xmldsql_cleanup_conn(XMLDWork *work) {
}

XMLDStatus xmldsql_prepare(XMLDWork *work) {
 return XMLD_SUCCESS;
}

void xmldsql_cleanup(XMLDWork *work) {
}

XMLDStatus xmldsql_parse(XMLDWork *work, char *query) {
 return XMLD_SUCCESS;
}

XMLDStatus xmldsql_walk(XMLDWork *work) {
 return XMLD_SUCCESS;
}

char *xmldsql_get_response(XMLDWork *work) {
 return NULL;
}
