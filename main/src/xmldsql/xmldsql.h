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

#ifndef __XMLDSQL_H
#define __XMLDSQL_H

void xmldsql_init(void);
void xmldsql_destroy(void);
XMLDStatus xmldsql_prepare_conn(XMLDWork *);
void xmldsql_cleanup_conn(XMLDWork *);
XMLDStatus xmldsql_prepare(XMLDWork *);
void xmldsql_cleanup(XMLDWork *);
XMLDStatus xmldsql_parse(XMLDWork *, char *);
XMLDStatus xmldsql_walk(XMLDWork *);
char *xmldsql_get_response(XMLDWork *);
	
#endif /* __XMLDSQL_H */
