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

#ifndef __XMLD_PARSER_H
#define __XMLD_PARSER_H

/* Represents a query parser */
struct XMLDInterafce {
 char *name; /* Parser name */
 void (*init) (void);
 void (*destroy) (void);
 XMLDStatus (*prepare_conn) (XMLDWork *);
 void (*cleanup_conn) (XMLDWork *);
 XMLDStatus (*prepare) (XMLDWork *);
 void (*cleanup) (XMLDWork *);
 XMLDStatus (*parse) (XMLDWork *, char *query);
 XMLDStatus (*walk) (XMLDWork *);
 char *(*get_response) (XMLDWork *);
};

typedef struct XMLDInterface XMLDInterface;
typedef struct XMLDList XMLDInterfaceList;

XMLDInterface *XMLDInterface_create(void);
void XMLDInterface_free(XMLDParser *);
void XMLDInterface_free_content(void *);

/* List functions */

XMLDInterfaceList *XMLDInterfaceList_create(void);
XMLDInterface *XMLDInterfaceList_add(XMLDInterfaceList *);

#endif /* __XMLD_PARSER_H */
