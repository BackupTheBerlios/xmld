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

#ifndef __XMLD_ASSOC_WALKER_H
#define __XMLD_ASSOC_WALKER_H

#define XMLD_ASSOC_WALKER_END 0
#define XMLD_ASSOC_WALKER_CONT 1

struct XMLDAssocWalker {
 XMLDAssoc *subject;
 int curr_index; 
};

typedef struct XMLDAssocWalker XMLDAssocWalker;

XMLDAssocWalker *XMLDAssocWalker_create(XMLDAssoc *);
int XMLDAssocWalker_next(XMLDAssocWalker *);
int XMLDAssocWalker_prev(XMLDAssocWalker *);
int XMLDAssocWalker_reset(XMLDAssocWalker *);
int XMLDAssocWalker_get_current_index(XMLDAssocWalker *);
void *XMLDAssocWalker_get_current_data(XMLDAssocWalker *);
void XMLDAssocWalker_free(XMLDAssocWalker *);

#endif /* XMLD_ASSOC_WALKER_H */
