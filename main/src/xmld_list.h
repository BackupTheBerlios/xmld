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
 
#ifndef HAVE_XMLD_LIST_H
#define HAVE_XMLD_LIST_H

struct XMLDList {
 void *content;
 void *last_element;
 void *curr_element; 
};

typedef struct XMLDList XMLDList;

XMLDList *XMLDList_create(int);
void XMLDList_free(XMLDList *);
void *XMLDList_add(XMLDList *);
void XMLDList_first(XMLDList *);
void XMLDList_last(XMLDList *);
short XMLDList_next(XMLDList *);
short XMLDList_prev(XMLDList *);
void *XMLDList_curr(XMLDList *);

#endif /* HAVE_XMLD_LIST_H */