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

void XMLDCol_free_content(XMLDCol *)
void XMLDCol_free(XMLDCol *);

typedef struct XMLDCol XMLDCol;

#endif /* HAVE_XMLD_COL_H */
