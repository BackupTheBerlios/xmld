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
#include <sys/file.h>

/*
 * Get a file decriptor and obtain a shared lock on its inode.
 */
FILE *fmanager_get_sh_fd(char *file) {
 FILE *ret=fopen(file, "r+");
 if (ret == NULL) {
  return NULL;
 }
 if (flock(fileno(ret), LOCK_SH) == 0) {
  return ret;
 }
 else {
  fclose(ret);
  return NULL;
 }
}

/*
 * Get a file descriptor and obtain an execlsive lock on its inode.
 */
FILE *fmanager_get_ex_fd(char *file) {
 FILE *ret=fopen(file, "r+");
 if (ret == NULL) {
  return NULL;
 }
 if (flock(fileno(ret), LOCK_EX) == 0) {
  return ret;
 }
 else {
  fclose(ret);
  return NULL;
 }
}

/*
 * : Unlocks the given file descriptor.
 * fd: The file descriptor to unlock.
 * returns: Whether successful.
 */
short fmanager_unlock_fd(FILE *fd) {
 if (flock(fileno(fd), LOCK_UN) == 0) {
  return 1;
 }
 else {
  return 0;
 }
}
