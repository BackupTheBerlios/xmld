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
 void *data_source; /* the data_source (fd/memory tree/whatever) */
 void *store; /* a pointer to whatever data the engine in use would
	       like to keep in a per-request way */
}; 

typedef struct XMLDResource XMLDResource;

XMLDResource *XMLDResource_create(void);
void XMLDResource_free(XMLDResource *);
void XMLDResource_free_content(void *);

/* List functions */

XMLDList *XMLDResource_create_list(void);
XMLDResource *XMLDResource_add_to_list(XMLDList *);

#endif /* __XMLD_RESOURCE_H */
