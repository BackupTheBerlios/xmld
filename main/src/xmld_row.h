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
 XMLDCol *cols;
 XMLDCol *last_col; /* A pointer to the last column */
};

typedef struct XMLDRow XMLDRow;

void XMLDRow_add_col(XMLDRow *);
void XMLDRow_fill_col(XMLDRow *, char *);
void XMLDRow_free_content(XMLDRow *);
void XMLDRow_free(XMLDRow *);

#endif /* HAVE_XMLD_ROW_H */
