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
 
#ifndef HAVE_XMLD_ROW_TABLE_H
#define HAVE_XMLD_ROW_TABLE_H

struct XMLDRowTable {
 XMLDRow *content;
 XMLDRow *last_element;
 XMLDRow *curr_element; 
};

typedef struct XMLDRowTable XMLDRowTable;

XMLDRowTable *XMLDRowTable_create(void);
void XMLDRowTable_free_content(XMLDRowTable *);
void XMLDRowTable_free(XMLDRowTable *);
void XMLDRowTable_add(XMLDRowTable *);
void XMLDRowTable_reset(XMLDRowTable *);
void XMLDRowTable_next(XMLDRowTable *);
void XMLDRowTable_prev(XMLDRowTable *);
XMLDRow *XMLDRowTable_curr(XMLDRowTable *);

#endif /* HAVE_XMLD_ROW_TABLE_H */