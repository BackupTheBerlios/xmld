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
#include "interface_list.h"

#ifdef USE_XMLD_SQL
#include "xmldsql/xmldsql.h"
#endif /* USE_XMLD_SQL */
 
XMLDStatus interface_list_init() {
 interface_list=XMLDinterfaceList_create();
 XMLDInterface *curr_interface;
#ifdef USE_XMLD_SQL
 curr_interface=XMLDInterfaceList_add(interface_list, "XMLDSQL");
 curr_interface->init=xmldsql_init;
 curr_interface->destroy=xmldsql_destroy;
 curr_interface->prepare_conn=xmldsql_prepare_conn;
 curr_interface->cleanup_conn=xmldsql_cleanup_conn;
 curr_interface->prepare=xmldsql_prepare;
 curr_interface->destroy=xmldsql_destroy;
 curr_interface->parse=xmldsql_parse;
 curr_interface->walk=xmldsql_walk;
 curr_interface->get_response=xmldsql_get_response;
#endif /* USE_XMLD_SQL */
 XMLDList_reset(interface_list);
 while (XMLDList_next(interface_list)) {
  (*(((XMLDInterface *) XMLDList_curr(interface_list))->init)) ();
 }
 
 return XMLD_SUCCESS;
}

XMLDStatus interface_list_shutdown() {
 XMLDList_reset(interface_list);
 while (XMLDList_next(interface_list)) {
  (*(((XMLDInterface *) XMLDList_curr(interface_list))->destroy)) ();
 }
 XMLDList_free(interface_list);
 return XMLD_SUCCESS;
}
