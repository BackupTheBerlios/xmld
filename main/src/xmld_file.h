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
 void *file;
 XMLDAssoc *data;
};

#ifndef XMLDFILE_TYPE_DEFINED
#define XMLDFILE_TYPE_DEFINED
 typedef struct XMLDFile XMLDFile;
#endif /* XMLDFILE_TYPE_DEFINED */

XMLDFile *XMLDFile_create(char *);
void XMLDFile_free(XMLDFile *);

#endif /* __XMLD_FILE_H */
