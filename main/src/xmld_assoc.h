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

#ifndef __XMLD_ASSOC_H
#define __XMLD_ASSOC_H

struct XMLDAssoc {
 void **values;
 char **keys;
 int length;
};

typedef struct XMLDAssoc XMLDAssoc;

XMLDAssoc *XMLDAssoc_create();
void XMLDAssoc_add(XMLDAssoc *, char *, void *);
void XMLDAssoc_remove(XMLDAssoc *, char *);
void *XMLDAssoc_get(XMLDAssoc *, char *);
void XMLDAssoc_remove_index(XMLDAssoc *, int);
int XMLDAssoc_get_index(XMLDAssoc *, char *);
void *XMLDAssoc_get_by_index(XMLDAssoc *, int);
char *XMLDAssoc_get_key_by_index(XMLDAssoc *, int);
void XMLDAssoc_free(XMLDAssoc *);

#endif /* __XMLD_ASSOC_H */
