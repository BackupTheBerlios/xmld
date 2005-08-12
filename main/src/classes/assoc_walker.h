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

#ifndef __ASSOC_WALKER_H
#define __ASSOC_WALKER_H

#define ASSOC_WALKER_END 0
#define ASSOC_WALKER_CONT 1

struct AssocWalker {
 Assoc *subject;
 int curr_index; 
};

typedef struct AssocWalker AssocWalker;

AssocWalker *AssocWalker_create(Assoc *);
int AssocWalker_next(AssocWalker *);
int AssocWalker_prev(AssocWalker *);
int AssocWalker_reset(AssocWalker *);
int AssocWalker_get_current_index(AssocWalker *);
void *AssocWalker_get_current_data(AssocWalker *);
void AssocWalker_free(AssocWalker *);

#endif /* ASSOC_WALKER_H */
