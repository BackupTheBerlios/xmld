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
 
#ifndef HAVE_XMLD_RESOURCE_H
#define HAVE_XMLD_RESOURCE_H

/* carries per-request resources */
struct XMLDResource {
 XMLDEngine *engine; /* the engine to be used */
 void *data_source; /* the data_source (fd/memory tree) */
}; 

typedef struct XMLDResource XMLDResource;

void XMLDResource_free_content(XMLDResource *);
void XMLDResource_free(XMLDResource *);

#endif /* HAVE_XMLD_RESOURCE_H */