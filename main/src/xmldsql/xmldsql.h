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

int yyparse(void *);
int yywrap(void);
int yerror(char *);

struct yy_buffer_state;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string(const char *);
void yy_switch_to_buffer(YY_BUFFER_STATE);
void yy_delete_buffer(YY_BUFFER_STATE);

#endif /* __XMLDSQL_H */
