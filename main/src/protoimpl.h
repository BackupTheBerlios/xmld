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

#ifndef __PROTOIMPL_H
#define __PROTOIMPL_H

#define HEADER_LENGTH 35
#define HEADER_FIELDS 2
#define HEADER_MESSAGE_LENGTH_ARG "message-length"
#define HEADER_STATUS_ARG "status"
#define HEADER_MESSAGE_LENGTH_LEN 7
#define HEADER_MAX_STATUS_LEN 1
#define DISCONNECTION_MESSAGE "DISCONNECT"

char *protoimpl_compose_msg(char **, char **, int, int);
int protoimpl_parse_msg(char *, char **, char **, int);
char *protoimpl_compose_header(int, int);
XMLDStatus protoimpl_parse_header(char *, int *, int *);
char *protoimpl_read_sequence(int, int *);
XMLDStatus protoimpl_write_sequence(int, char *, int);

#endif /* __PROTOIMPL_H */