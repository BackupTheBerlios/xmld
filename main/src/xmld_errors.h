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
 XMLD_ENOTIMPL
} xmld_errno;

char *err_str[6]; /* = {
 "The request file was not found.",
 "The given engine does not exist.",
 "Invalid file type.",
 "Invalid use of an aggregate expression.",
 "An error has occured while parsing the given query.",
 "The given query is not yet implemented."
};*/

#define ERROR_RESPONSE xmld_socket_write(work->conn->fd, err_str[xmld_errno])

void init_error_array(void);

#endif /* __XMLD_ERRORS_H */

