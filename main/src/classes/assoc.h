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
};

typedef struct Assoc Assoc;

Assoc *Assoc_create(void);
void Assoc_add(Assoc *, char *, void *);
void Assoc_remove(Assoc *, char *);
void *Assoc_get(Assoc *, char *);
void *Assoc_get_key_index(Assoc *, char *, int);
int Assoc_get_length(Assoc *);
void Assoc_remove_index(Assoc *, int);
int Assoc_get_index(Assoc *, char *);
void *Assoc_get_by_index(Assoc *, int);
void Assoc_update_key(Assoc *, char *, char *);
void Assoc_update_key_by_index(Assoc *, int, char *);
void Assoc_free(Assoc *);

#endif /* __ASSOC_H */
