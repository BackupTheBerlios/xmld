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

#ifndef HAVE_XMLD_ERRORS_H
#define HAVE_XMLD_ERRORS_H

enum xmld_error {
 XMLD_EPREPAREFAIL,
 XMLD_ENOENGINE, 
} xmld_errno;

char *err_str[2] = {
 "An error has occured while preparing the engine.",
 "The given engine does not exist."
};

#define ERROR_RESPONSE xmld_socket_write(work->conn->fd, err_str[xmld_errno])

#endif /* HAVE_XMLD_ERRORS_H */

