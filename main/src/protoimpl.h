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

char *protoimpl_compose_msg(char **, char **, int, int);
int protoimpl_parse_msg(char *, const char **, const char **, int);
char *protoimpl_read_sequence(int, XMLDStatus *);
XMLDStatus protoimpl_write_sequence(int, char *, XMLDStatus);

#endif /* __PROTOIMPL_H */
