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

#include "includes.h";

/*
 * Creates a new associative table walker structure for the
 * given associative table parameter.
 */
XMLDAssocWalker *XMLDAssocWalker_create(XMLDAssoc *subject) {
 XMLDAssocWalker *walker = (XMLDAssocWalker *) malloc(sizeof(XMLDAssocWalker));
 walker->subject = subject;
 walker->curr_index = -1;
}

/*
 * Walks to the next element in the associative table.
 */
int XMLDAssocWalker_next(XMLDAssocWalker *walker) {
 if (walker->curr_index >= walker->subject->length) {
  return XMLD_ASSOC_WALKER_END;
 }
 else {
  walker->curr_index++;
  return XMLD_ASSOC_WALKER_CONT;
 }
}

/*
 * Walks to the previous element in the associative table.
 */
int XMLDAssocWalker_prev(XMLDAssocWalker *walker) {
 if (walker->curr_index <= 0) {
  return XMLD_ASSOC_WALKER_END;
 }
 else {
  walker->curr_index--;
  return XMLD_ASSOC_WALKER_CONT;
 }
}


/*
 * Resets the associative table walker.
 */
void XMLDAssocWalker_reset(XMLDAssocWalker *walker) {
 walker->curr_index = -1;
}

/*
 * Gets the current index of the walker.
 */
int XMLDAssocWalker_get_current_index(XMLDAssocWalker *walker) {
 return walker->curr_index;
}

/*
 * Gets the value stored at the current walker index in the
 * associative table.
 */
void *XMLDAssocWalker_get_current_data(XMLDAssocWalker *walker) {
 if (walker->curr_index  >= 0) {
  return walker->subject->data + walker->curr_index;
 }
 else {
  return NULL;
 }
}

/*
 * Frees the memory used by a walker structure.
 */
void XMLDAssocWalker_free(XMLDAssocWalker *walker) {
 free(walker);
}
