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
 
#ifndef HAVE_XMLD_COL_TABLE_H
#define HAVE_XMLD_COL_TABLE_H

struct XMLDColTable {
 XMLDCol *content;
 XMLDCol *last_element;
 XMLDCol *curr_element; 
};

typedef struct XMLDColTable XMLDColTable;

XMLDColTable *XMLDColTable_create(void);
void XMLDColTable_free_content(XMLDColTable *);
void XMLDColTable_free(XMLDColTable *);
void XMLDColTable_add(XMLDColTable *, char *, short);
void XMLDColTable_reset(XMLDColTable *);
void XMLDColTable_next(XMLDColTable *);
void XMLDColTable_prev(XMLDColTable *);
XMLDCol *XMLDColTable_curr(XMLDColTable *);

#endif /* HAVE_XMLD_COL_TABLE_H */