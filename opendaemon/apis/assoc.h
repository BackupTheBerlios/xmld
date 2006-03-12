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

#ifndef __ASSOC_H
#define __ASSOC_H

#define ASSOC_MAX_FREE 10

struct Assoc {
 void **values;
 long *keys;
 int length;
 int array_length;
 Bool integer_keys;
};

typedef struct Assoc Assoc;

/* Creation and parameter setting */
Assoc *Assoc_create(void);
Status Assoc_set_integer_keys(Assoc *, Bool);

/* Addition and removal of data */
void Assoc_add(Assoc *, void *, void *);
Status Assoc_remove(Assoc *, void *);
Status Assoc_remove_index(Assoc *, int);

/* Getters */
void *Assoc_get(Assoc *, void *);
void *Assoc_get_by_index(Assoc *, int);
void *Assoc_get_key_index(Assoc *, void *, int);
int Assoc_get_index(Assoc *, void *);

/* Setter */
Status Assoc_set(Assoc *, void *, void *);

int Assoc_get_length(Assoc *);

/* Key update */
Status Assoc_update_key(Assoc *, void *, void *);
Status Assoc_update_key_by_index(Assoc *, int, void *);

/* Memory freeing routine */
void Assoc_free(Assoc *);

#endif /* __ASSOC_H */
