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

#ifndef __XMLD_ERRORS_H
#define __XMLD_ERRORS_H

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

#define ERROR_RESPONSE xmld_socket_write(work->conn->fd, err_str[xmld_errno])
#define XMLD_SUCCESS 1
#define XMLD_FAILURE 0
#define XMLD_SPECIAL -1

void init_error_array(void);

#endif /* __XMLD_ERRORS_H */

