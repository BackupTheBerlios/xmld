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

#ifndef __XMLD_INTERFACE_H
#define __XMLD_INTERFACE_H

/* Represents a user interface */
struct XMLDInterface {
 char *name; /* Interface name */
 void (*init) (void);
 void (*destroy) (void);
 XMLDStatus (*prepare_conn) (XMLDWork *);
 void (*cleanup_conn) (XMLDWork *);
 XMLDStatus (*prepare) (XMLDWork *);
 void (*cleanup) (XMLDWork *);
 XMLDStatus (*parse) (XMLDWork *, char *);
 XMLDStatus (*walk) (XMLDWork *);
 char *(*get_response) (XMLDWork *);
};

#ifndef XMLDINTERFACE_TYPE_DEFINED
#define XMLDINTERFACE_TYPE_DEFINED
 typedef struct XMLDInterface XMLDInterface;
 typedef struct XMLDList XMLDInterfaceList;
#endif /* XMLDINTERFACE_TYPE_DEFINED */

XMLDInterface *XMLDInterface_create(char *);
void XMLDInterface_free(XMLDInterface *);
void XMLDInterface_free_content(void *);

/* List functions */

XMLDInterfaceList *XMLDInterfaceList_create(void);
XMLDInterface *XMLDInterfaceList_add(XMLDInterfaceList *, char *);
XMLDInterface *XMLDInterfaceList_search_by_name(XMLDInterfaceList *, char *);
#endif /* __XMLD_INTERFACE_H */
