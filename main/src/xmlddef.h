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

#ifndef __XMLDDEF_H
#define __XMLDDEF_H

/* XMLDStatus values */
#define XMLD_SUCCESS 1
#define XMLD_FAILURE 0
#define XMLD_SPECIAL -1

/* XMLDBool values */
#define XMLD_FALSE 0
#define XMLD_TRUE 1

#define BIT_ISSET(bitmask, bit) ((bitmask) & (bit))
#define ERROR_RESPONSE xmld_socket_write(work->conn->fd, err_str[xmld_errno])

typedef int XMLDBool;
typedef int XMLDStatus;

enum xmld_error {
 XMLD_ENOFILE,
 XMLD_ENOENGINE,
 XMLD_EINVALFILE,
 XMLD_ENOAGGR,
 XMLD_EPARSE,
 XMLD_ENOTIMPL,
 XMLD_ENOFORMAT
} xmld_errno;

char *err_str[7];

#endif /* __XMLDDEF_H */
