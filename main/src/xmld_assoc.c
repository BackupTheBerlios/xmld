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
#include <string.h>
#include "includes.h"

/*
 * Creates a new associative table structure.
 */ 
XMLDAssoc *XMLDAssoc_create(void) {
 XMLDAssoc *assoc = (XMLDAssoc *) malloc(sizeof(XMLDAssoc));
 assoc->values = NULL;
 assoc->keys = NULL;
 assoc->length = 0;
 assoc->array_length = 0;
 return assoc;
}

/*
 * Gets the number of elements currently in the table.
 */
int XMLDAssoc_get_length(XMLDAssoc *assoc) {
 return assoc->length;
}

/*
 * Adds an element to the given associative table.
 * key and value are to be allocated and freed externally.
 */
void XMLDAssoc_add(XMLDAssoc *assoc, char *key, void *data) {
 if (assoc->array_length > assoc->length) {
  assoc->values[assoc->length] = data;
  assoc->keys[assoc->length] = hash(key);
  assoc->length++;
 }
 else {
  assoc->values = (void **) realloc(assoc->values, ++(assoc->length) * sizeof(void *));
  assoc->keys = (long *) realloc(assoc->keys, assoc->length * sizeof(long));
  assoc->array_length++;
  assoc->values[assoc->length-1] = data;
  assoc->keys[assoc->length-1] = hash(key);
 }
}

/*
 * Gets the index of the element with that has the given key.
 * -1 = not found
 */
int XMLDAssoc_get_index(XMLDAssoc *assoc, char *key) {
 int i = -1;
 int hash_key = hash(key);
 for (i = 0; i < assoc->length; i++) {
  if (assoc->keys[i] == hash_key) {
   break;
  }
 }
 return i;
}

/*
 * Removes an element that has the given key from the given 
 * associative table.
 */
void XMLDAssoc_remove(XMLDAssoc *assoc, char *key) {
 XMLDAssoc_remove_index(assoc, XMLDAssoc_get_index(assoc, key));  
}

/*
 * Removes the element that has the given index from the associative
 * table.
 */
void XMLDAssoc_remove_index(XMLDAssoc *assoc, int index) {
 if (index != -1) {
  assoc->values[index] = NULL;
  assoc->keys[index] = NULL;
  assoc->length--;
  if (assoc->length - index > 0) {
   memmove(assoc->values + index, assoc->values + assoc->length, assoc->length - index);
   memmove(assoc->keys + index, assoc->keys + assoc->length, assoc->length - assoc->index);
  }
  if (assoc->array_length - assoc->length > XMLD_ASSOC_MAX_FREE) {
   assoc->values = (void **) realloc(assoc->values, --assoc->array_length * sizeof(void *));
   assoc->keys = (long *) realloc(assoc->keys, assoc->array_length * sizeof(long));
  }
 }
}

/*
 * Returns the data associated to the given key -- NULL if not found.
 */
void *XMLDAssoc_get(XMLDAssoc *assoc, char *key) {
 int i;
 int hash_key = hash(key);
 for (i = 0; i < assoc->length; i++) {
  if (assoc->keys[i] == hash_key) {
   return assoc->values[i];
  }
 }
 return NULL;
}

/*
 * Returns the data associated to the given key -- NULL if not found.
 */
void *XMLDAssoc_get_key_index(XMLDAssoc *assoc, char *key, int index) {
 int i;
 int num = 0;
 int hash_key = hash(key);
 for (i = 0; i < assoc->length; i++) {
  if (assoc->keys[i] == hash_key) {
   if (num == index) {
    return assoc->values[i];
   }
   num++;
  }
 }
 return NULL;
}

/*
 * Returns the data associated to the given index -- NULL if the index
 * is out of bounds.
 */
void *XMLDAssoc_get_by_index(XMLDAssoc *assoc, int index) {
 if (index > assoc->length-1 || index < 0) {
  return NULL;
 }
 return assoc->values[index];
}

/*
 * Updates the hashed key of an element in the associative table.
 */
void XMLDAssoc_update_key(XMLDAssoc *assoc, char *key, char *new_key) {
 int index = XMLDAssoc_get_index(assoc, key);
 assoc->keys[index] = hash(new_key);
}

/*
 * Updates the hashed key of an element in the associative table
 * given its index.
 */
void XMLDAssoc_update_key_by_index(XMLDAssoc *assoc, int index, char *new_key) {
 if (index < 0 || index	>= assoc->length) {
  return;
 }
 assoc->keys[index] = hash(new_key);
}

/*
 * Frees the memory used by the associative table structure itself
 * and not its elements.
 */
void XMLDAssoc_free(XMLDAssoc *assoc) {
 if (assoc->array_length > 0) {
  free(assoc->values);
  free(assoc->keys);
 }
 free(assoc);
}
