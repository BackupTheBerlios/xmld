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

#ifndef __XMLD_FILE_ASSOC_H
#define __XMLD_FILE_ASSOC_H

struct XMLDFileAssoc {
 char *file_name;
 FILE *fd;
};

typedef struct XMLDFileAssoc XMLDFileAssoc;

XMLDFileAssoc *XMLDFileAssoc_create(char *, FILE *);
void XMLDFileAssoc_free(XMLDFileAssoc *);
void XMLDFileAssoc_free_content(void *);

/* List functions */

XMLDList *XMLDFileAssoc_create_list(void);
XMLDFileAssoc *XMLDFileAssoc_add_to_list(XMLDList *, char *, FILE *);
XMLDFileAssoc *XMLDFileAssoc_search_list_by_file_name(XMLDList *, char *);

#endif /* __XMLD_FILE_ASSOC_H */
