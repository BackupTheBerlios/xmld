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
 * Creates a new associative table structure.
 */ 
Assoc *Assoc_create(void) {
 Assoc *assoc = (Assoc *) malloc(sizeof(Assoc));
 assoc->values = NULL;
 assoc->keys = NULL;
 assoc->length = 0;
 assoc->array_length = 0;
 assoc->integer_keys = FALSE;
 return assoc;
}

/*
 * Sets the integer keys mode flag of the associative table.
 */
Status Assoc_set_integer_keys(Assoc *assoc, Bool val) {
 if (assoc->values == NULL && assoc->keys == NULL && assoc->length == 0 && assoc->array_length == 0) {
  assoc->integer_keys = val;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

/*
 * Gets the number of elements currently in the table.
 */
int Assoc_get_length(Assoc *assoc) {
 return assoc->length;
}

/*
 * Adds an element to the given associative table.
 * key and value are to be allocated and freed externally.
 */
void Assoc_add(Assoc *assoc, void *key, void *data) {
 if (assoc->array_length > assoc->length) {
  assoc->values[assoc->length] = data;
  assoc->keys[assoc->length] = (assoc->integer_keys) ? (int) key : hash((char *) key);
  assoc->length++;
 }
 else {
  assoc->values = (void **) realloc(assoc->values, ++(assoc->length) * sizeof(void *));
  assoc->keys = (long *) realloc(assoc->keys, assoc->length * sizeof(long));
  assoc->array_length++;
  assoc->values[assoc->length-1] = data;
  assoc->keys[assoc->length - 1] = (assoc->integer_keys) ? (int) key : hash((char *) key);
 }
}

/*
 * Gets the index of the element that has the given key.
 * -1 = not found
 */
int Assoc_get_index(Assoc *assoc, void *key) {
 int i = -1;
 int hash_key = (assoc->integer_keys) ? (int) key : hash((char *) key);
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
void Assoc_remove(Assoc *assoc, void *key) {
 Assoc_remove_index(assoc, Assoc_get_index(assoc, key));  
}

/*
 * Removes the element that has the given index from the associative
 * table.
 */
void Assoc_remove_index(Assoc *assoc, int index) {
 if (index != -1) {
  assoc->values[index] = NULL;
  assoc->keys[index] = 0;
  assoc->length--;
  if (assoc->length - index > 0) {
   memmove(assoc->values + index, assoc->values + assoc->length, assoc->length - index);
   memmove(assoc->keys + index, assoc->keys + assoc->length, assoc->length - index);
  }
  if (assoc->array_length - assoc->length > ASSOC_MAX_FREE) {
   assoc->values = (void **) realloc(assoc->values, --assoc->array_length * sizeof(void *));
   assoc->keys = (long *) realloc(assoc->keys, assoc->array_length * sizeof(long));
  }
 }
}

/*
 * Returns the data associated to the given key -- NULL if not found.
 */
void *Assoc_get(Assoc *assoc, void *key) {
 int i;
 int hash_key = (assoc->integer_keys) ? (int) key : hash((char *) key);
 for (i = 0; i < assoc->length; i++) {
  if (assoc->keys[i] == hash_key) {
   return assoc->values[i];
  }
 }
 return NULL;
}

/*
 * Sets the data associated to the given key to the given value
 * -- If the key is not found FAILURE is returned.
 */
Status Assoc_set(Assoc *assoc, void *key, void *data) {
 int index = Assoc_get_index(assoc, key);

 if (index == -1) {
  return FAILURE;
 }

 assoc->values[index] = data;
 return SUCCESS;
}

/*
 * Returns the data associated to the given key -- NULL if not found.
 */
void *Assoc_get_key_index(Assoc *assoc, void *key, int index) {
 int i;
 int num = 0;
 int hash_key = (assoc->integer_keys) ? (int) key : hash((char *) key);
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
void *Assoc_get_by_index(Assoc *assoc, int index) {
 if (index > assoc->length-1 || index < 0) {
  return NULL;
 }
 return assoc->values[index];
}

/*
 * Updates the hashed key of an element in the associative table.
 */
void Assoc_update_key(Assoc *assoc, void *key, void *new_key) {
 int index = Assoc_get_index(assoc, key);
 assoc->keys[index] = (assoc->integer_keys) ? (int) new_key : hash((char *) new_key);
}

/*
 * Updates the hashed key of an element in the associative table
 * given its index.
 */
void Assoc_update_key_by_index(Assoc *assoc, int index, char *new_key) {
 if (index < 0 || index	>= assoc->length) {
  return;
 }
 assoc->keys[index] = (assoc->integer_keys) ? (int) new_key : hash((char *) new_key);
}

/*
 * Frees the memory used by the associative table structure itself
 * and not its elements.
 */
void Assoc_free(Assoc *assoc) {
 if (assoc != NULL) {
  if (assoc->array_length > 0) {
   free(assoc->values);
   free(assoc->keys);
  }
  free(assoc);
 }
}
