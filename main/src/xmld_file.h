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
 char *name;
 char *path;
 void *data;
 XMLDFile *store; /* Additional storage field   */
 XMLDEngine *engine;
 int level;
 int priv;
 fpos_t node_pos;
};

#ifndef XMLDFILE_TYPE_DEFINED
#define XMLDFILE_TYPE_DEFINED
 typedef struct XMLDFile XMLDFile;
#endif /* XMLDFILE_TYPE_DEFINED */

XMLDFile *XMLDFile_create(char *);
void XMLDFile_free(XMLDFile *);

#endif /* __XMLD_FILE_H */
