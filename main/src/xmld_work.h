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

#ifndef __XMLD_WORK_H
#define __XMLD_WORK_H

struct XMLDWork {
 XMLDInterface *interface;
 XMLDAssoc *files;
 XMLDAssoc *data;
};

typedef struct XMLDWork XMLDWork;

XMLDWork *XMLDWork_create(void);
char *XMLDWork_get_full_file(XMLDWork *);
void XMLDWork_free(XMLDWork *);

#endif /* __XMLD_WORK_H */
