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

#ifndef HAVE_XMLD_TABLE_H
#define HAVE_XMLD_TABLE_H

struct XMLDTable {
 XMLDList *list;
 void *fill; /* the current empty segment (which can be used) */
 int num; /* Number of elements */
 int used; /* Number of used elements, this value is maintained externally
	    * because there isn't a removal routine in the table functions
	    * it's originally put here to fit the shared memory segment in
	    * sosel. */
};

typedef struct XMLDTable XMLDTable;

XMLDTable *XMLDTable_create(int, int, void (*) (void*));
void XMLDTable_free(XMLDTable *);
void XMLDTable_free_content(void *);
void *XMLDTable_add(XMLDTable *);
void XMLDTable_reset(XMLDTable *);
void *XMLDTable_first(XMLDTable *);
void *XMLDTable_last(XMLDTable *);
short XMLDTable_next(XMLDTable *);
short XMLDTable_prev(XMLDTable *);
void *XMLDTable_curr(XMLDTable *);

#endif /* HAVE_XMLD_TABLE_H */