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

#define HEADER_LENGTH 33
#define HEADER_FIELDS 2
#define HEADER_MESSAGE_LENGTH_ARG "message-length"
#define HEADER_STATUS_ARG "status"
#define HEADER_MESSAGE_LENGTH_LEN 7
#define HEADER_MAX_STATUS_LEN 1
#define DISCONNECTION_MESSAGE "DISCONNECT"
#define COL_SEP_FIELD "col-sep"
#define COL_SEP_ENC_FIELD "col-sep-enc"
#define ROW_SEP_FIELD "row-sep"
#define ROW_SEP_ENC_FIELD "row-sep-enc"
#define DOWN_LEVEL_FIELD "down-level"
#define DOWN_LEVEL_ENC_FIELD "down-level-enc"
#define UP_LEVEL_FIELD "up-level"
#define UP_LEVEL_ENC_FIELD "up-level-enc"
#define INIT_NUM_FIELDS 8
#define USER_NAME_FIELD "user-name"
#define PASS_FIELD "pass"
#define ERROR_NUM_FIELD "ERR"
#define ERROR_MSG_FIELD "MSG"
#define ERROR_FIELDS 2
#define ERROR_NUM_LENGTH 5

char *protoimpl_compose_msg(char **, char **, int, int);
int protoimpl_parse_msg(char *, char **, char **, int);
char *protoimpl_compose_header(int, int);
XMLDStatus protoimpl_parse_header(char *, int *, int *);
char *protoimpl_read_sequence(int, int *);
XMLDStatus protoimpl_write_sequence(int, char *, int);
XMLDStatus protoimpl_write_error_sequence(int, int, char *);

#endif /* __PROTOIMPL_H */
