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
 
#ifndef __XMLD_RESOURCE_H
#define __XMLD_RESOURCE_H

/* carries per-request resources */
struct XMLDResource {
 XMLDEngine *engine; /* the engine to be used */
 XMLDFileList *files; /* files used by the request */
}; 

typedef struct XMLDResource XMLDResource;
typedef XMLDList XMLDResourceList;

XMLDResource *XMLDResource_create(void);
void XMLDResource_free(XMLDResource *);
void XMLDResource_free_content(void *);

/* List functions */

XMLDResourceList *XMLDResourceList_create(void);
XMLDResource *XMLDResourceList_add(XMLDResourceList *);

#endif /* __XMLD_RESOURCE_H */
