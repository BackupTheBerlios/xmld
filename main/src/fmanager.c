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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "xmld_list.h"
#include "xmld_file_assoc.h"

/*
 * Initializes the fmanager.
 */
short fmanager_init() {
}

/*
 * : Adds a new element to file_list and sets
 * its file_name filed to the given file_name
 * and associates the given fd to it.
 */
void fmanager_assoc_file_to_fd(char *file_name, FILE *fd) {
}

/*
 * : Returns the file descriptor associated
 * with the given file name in the file_list.
 * file: The file for which a file descriptor is
 * to be returned.
 * returns: the named file descriptor.
 * 
 * Note: this function blocks until the named file
 * is not locked for reading.
 */
FILE *fmanager_get_read_fd(char *file) { 
}

/*
 * : Returns the file descriptor associated
 * with the given file name in the file_list.
 * file: The file for which a file descriptor is
 * to be returned.
 * returns: the named file descriptor.
 * 
 * Note: this function blocks until the named file
 * is not locked for writing.
 */
FILE *fmanager_get_write_fd(char *file) {
}

/*
 * : Read-locks the given file descriptor.
 * fd: The file descriptor to lock.
 * returns: Whether successful.
 */
short fmanager_lock_read_fd(FILE *fd) {
}

/*
 * : Write-locks the given file descriptor.
 * fd: The file descriptor to lock.
 * returns: Whether successful.
 */
short fmanager_lock_write_fd(FILE *fd) {
}

/*
 * : Unlocks the given file descriptor.
 * fd: The file descriptor to unlock.
 * returns: Whether successful.
 */
short fmanager_unlock_fd(FILE *fd) {
}

/*
 * Shutdowns fmanager.
 * Note: This function fclose()'s each of
 * the file descriptors in file_list.
 */
short fmanager_shutdown() {
}
