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

#ifndef __XMLD_FILE_H
#define __XMLD_FILE_H

struct XMLDFile {
 char *name;  /* The name of the file       */
 void *data;  /* File's data representation */
 void *store; /* Additional storage field   */
 XMLDEngine *engine; /* The engine to be used for this file */
 int level; /* The current level in the file */
 fpos_t element_pos; /* The position of the beginning of the current element in the file */
};

#ifndef XMLDFILE_TYPE_DEFINED
#define XMLDFILE_TYPE_DEFINED
 typedef struct XMLDFile XMLDFile;
 typedef XMLDList XMLDFileList;
#endif /* XMLDFILE_TYPE_DEFINED */

XMLDFile *XMLDFile_create(char *);
char *XMLDFile_get_full_name(XMLDFile *, XMLDWork *);
void XMLDFile_free(XMLDFile *);
void XMLDFile_free_content(void *);

/* List functions */

XMLDFileList *XMLDFileList_create(void);
XMLDFile *XMLDFileList_add(XMLDFileList *, char *);
XMLDFile *XMLDFileList_search_by_name(XMLDFileList *, char *);
int XMLDFileList_get_max_level(XMLDFileList *);

#endif /* __XMLD_FILE_H */
