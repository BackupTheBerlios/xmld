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
 
#ifndef __XMLD_LIST_H
#define __XMLD_LIST_H

struct XMLDList {
 void *content;
 void *last_element;
 void *curr_element;
 int item_size;
 void (*free_func) (void*);
};

typedef struct XMLDList XMLDList;
typedef XMLDList XMLDListList;

XMLDList *XMLDList_create(int, void (*) (void*));
void XMLDList_free(XMLDList *);
void XMLDList_free_content(void *);
void *XMLDList_add(XMLDList *);
void XMLDList_reset(XMLDList *);
void *XMLDList_first(XMLDList *);
void *XMLDList_last(XMLDList *);
XMLDStatus XMLDList_next(XMLDList *);
XMLDStatus XMLDList_prev(XMLDList *);
void *XMLDList_curr(XMLDList *);
void default_free_func(void *);

/* List functions */
XMLDListList *XMLDListList_create();
XMLDList *XMLDListList_add(XMLDListList *, int, void (*) (void*));

#endif /* __XMLD_LIST_H */
