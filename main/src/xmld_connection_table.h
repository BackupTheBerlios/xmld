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
 
#ifndef HAVE_XMLD_CONNECTION_TABLE_H
#define HAVE_XMLD_CONNECTION_TABLE_H

struct XMLDConnectionTable {
 XMLDConnection *content;
 XMLDConnection *last_element;
 XMLDConnection *curr_element; 
};

typedef struct XMLDConnectionTable XMLDConnectionTable;

void XMLDConnectionTable_free_content(XMLDConnectionTable *);
void XMLDConnectionTable_free(XMLDConnectionTable *);
void XMLDConnectionTable_add(XMLDConnectionTable *, int, int, char *);
void XMLDConnectionTable_reset(XMLDConnectionTable *);
void XMLDConnectionTable_next(XMLDConnectionTable *);
void XMLDConnectionTable_prev(XMLDConnectionTable *);
XMLDConnection *XMLDConnectionTable_curr(XMLDConnectionTable *);

#endif /* HAVE_XMLD_CONNECTION_TABLE_H */