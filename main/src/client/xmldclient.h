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

#ifndef __XMLDCLIENT_H
#define __XMLDCLIENT_H

#define PROMPT "xmld> "

struct conn_info {
 char col_sep;
 char row_sep;
 char down_level;
 char up_level;
 char *col_sep_enc;
 char *row_sep_enc;
 char *down_level_enc;
 char *up_level_enc;
};

XMLDStatus xmldclient_auth(int);
XMLDStatus xmldclient_process_info_msg(char *, struct conn_info *);
void xmldclient_print_record_set(char *);
void xmldclient_print_error(char *);
void xmldclient_print_server_err_msg(char *);
void xmldclient_get_port(int *);

#endif /* __XMLDCLIENT_H */
