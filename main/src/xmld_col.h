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
 
#ifndef HAVE_XMLD_COL_H
#define HAVE_XMLD_COL_H

struct XMLDCol {
 char *val;
};

typedef struct XMLDCol XMLDCol;

XMLDCol *XMLDCol_create(char *, short); /* short is whether to copy the char **/
void XMLDCol_fill(XMLDCol *, char *, short);
void XMLDCol_free(XMLDCol *);

/* List functions */

XMLDList *XMLDCol_create_list(void);
void *XMLDCol_add_to_list(XMLDList *, char *, short);

#endif /* HAVE_XMLD_COL_H */
