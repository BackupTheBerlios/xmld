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
 
#ifndef HAVE_XMLD_ROW_H
#define HAVE_XMLD_ROW_H

/* Represents a row of response */
struct XMLDRow {
 XMLDList *cols;
};

typedef struct XMLDRow XMLDRow;

XMLDRow *XMLDRow_create(XMLDList *);
void XMLDRow_add_col(XMLDRow *);
void XMLDRow_fill_col(XMLDRow *, char *, short);
void XMLDRow_free(XMLDRow *);

/* List functions */

XMLDList *XMLDRow_create_list(void);
void XMLDRow_add_to_list(XMLDList *, XMLDList *);

#endif /* HAVE_XMLD_ROW_H */
