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

#include "../includes.h"

/*
 * Creates a new associative table walker structure for the
 * given associative table parameter.
 */
AssocWalker *AssocWalker_create(Assoc *subject) {
 AssocWalker *walker = (AssocWalker *) malloc(sizeof(AssocWalker));
 walker->subject = subject;
 walker->curr_index = -1;
 return walker;
}

/*
 * Walks to the next element in the associative table.
 */
int AssocWalker_next(AssocWalker *walker) {
 if (walker->curr_index >= walker->subject->length) {
  return ASSOC_WALKER_END;
 }
 else {
  walker->curr_index++;
  return ASSOC_WALKER_CONT;
 }
}

/*
 * Walks to the previous element in the associative table.
 */
int AssocWalker_prev(AssocWalker *walker) {
 if (walker->curr_index <= 0) {
  return ASSOC_WALKER_END;
 }
 else {
  walker->curr_index--;
  return ASSOC_WALKER_CONT;
 }
}


/*
 * Resets the associative table walker.
 */
void AssocWalker_reset(AssocWalker *walker) {
 walker->curr_index = -1;
}

/*
 * Gets the current index of the walker.
 */
int AssocWalker_get_current_index(AssocWalker *walker) {
 return walker->curr_index;
}

/*
 * Gets the value stored at the current walker index in the
 * associative table.
 */
void *AssocWalker_get_current_data(AssocWalker *walker) {
 if (walker->curr_index  >= 0) {
  return walker->subject->values + walker->curr_index;
 }
 else {
  return NULL;
 }
}

/*
 * Frees the memory used by a walker structure.
 */
void AssocWalker_free(AssocWalker *walker) {
 free(walker);
}
