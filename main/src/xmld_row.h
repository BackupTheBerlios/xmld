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
 
#ifndef __XMLD_ROW_H
#define __XMLD_ROW_H

/* Row types */
#define XMLD_ROW_NOTHING 0 /* Does nothing to levels   */
#define XMLD_ROW_DOWN 1    /* Goes one level deeper    */
#define XMLD_ROW_UP 2      /* Goes one level shallower */


/* Represents a row of response */
struct XMLDRow {
 int level_act;
 XMLDColList *cols;
};

typedef struct XMLDRow XMLDRow;
typedef XMLDList XMLDRowList;

XMLDRow *XMLDRow_create();
void XMLDRow_add_col(XMLDRow *);
void XMLDRow_fill_col(XMLDRow *, char *);
void XMLDRow_free(XMLDRow *);
void XMLDRow_free_content(void *);

/* List functions */

XMLDRowList *XMLDRowList_create(void);
XMLDRow *XMLDRowList_add(XMLDRowList *);

#endif /* __XMLD_ROW_H */
