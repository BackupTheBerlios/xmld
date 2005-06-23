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
 int port;
 void (*init) (XMLDInterface *, XMLDCfgDirective *);
 void (*destroy) (void);
 void (*user_connection) (void *);
 XMLDAssoc *data;
};

#ifndef XMLDINTERFACE_TYPE_DEFINED
#define XMLDINTERFACE_TYPE_DEFINED
 typedef struct XMLDInterface XMLDInterface;
#endif /* XMLDINTERFACE_TYPE_DEFINED */

XMLDInterface *XMLDInterface_create(char *);
void XMLDInterface_copy(XMLDInterface *, XMLDInterface *);
void XMLDInterface_free(XMLDInterface *);

#endif /* __XMLD_INTERFACE_H */
