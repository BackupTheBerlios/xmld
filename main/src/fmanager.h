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

#ifndef __FMANAGER_H
#define __FMANAGER_H

XMLDList *file_list;

short fmanager_init(void);
FILE *fmanager_get_read_fd(char *);
FILE *fmanager_get_write_fd(char *);
short fmanager_lock_read_fd(FILE *);
short fmanager_lock_write_fd(FILE *);
short fmanager_unlock_fd(FILE *);
short fmanager_shutdown(void);
void fmanager_assoc_file_to_fd(char *, FILE *);

#endif /* __FMANAGER_H */
