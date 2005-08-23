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

#ifndef __FILE_H
#define __FILE_H

struct File {
 void *file;
 Assoc *data;
};

#ifndef FILE_TYPE_DEFINED
#define FILE_TYPE_DEFINED
 typedef struct File File;
#endif /* FILE_TYPE_DEFINED */

File *File_create(void);
void File_free(File *);

#endif /* __FILE_H */
