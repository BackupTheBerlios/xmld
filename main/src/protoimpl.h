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

XMLDStatus protoimpl_parse_header(char *, char **);
char *protoimpl_create_header(char **, int);
int protoimpl_parse_err_msg(char *, char **);
char *protoimpl_create_err_msg(int, char *);
char *protoimpl_parse_user_msg(char *);
char *protoimpl_create_user_msg(char *);
char *protoimpl_parse_pass_msg(char *);
char *protoimpl_create_pass_msg(char *);

#endif /* __PROTOIMPL_H */
