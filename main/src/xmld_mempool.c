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
 
#include <stdlib.h>
#include "xmld_mempool.h"

int i;
struct XMLDMemPool *XMLDMemPool_create(int segsize, int num_seg) {
 struct XMLDMemPool *pool=(struct XMLDMemPool *)malloc(sizeof(struct XMLDMemPool));
 pool->num_seg=num_seg;
 pool->segsize=segsize;
 pool->pool=(void**) malloc(num_seg*sizeof(void*));
 pool->status=(short*) calloc(num_seg,sizeof(short));
 for (num_seg=0; num_seg < pool->num_seg; num_seg++) {
  pool->pool[num_seg]=malloc( segsize);
 }
 pool->num_used=0;
 return pool;
}

void XMLDMemPool_free(struct XMLDMemPool *pool) {
 for (i=0;i<pool->num_seg;i++) {
  free(pool->pool[i]);
 }
 free(pool->pool);
 free(pool->status);
 free(pool);
}

void *XMLDMemPool_get_segment(struct XMLDMemPool *pool) {
 if (pool->num_used < pool->num_seg) {
  for (i=0;i< pool->num_seg ;i++) {
   if (pool->status[i]==0) {
    pool->num_used++;
    pool->status[i]=1;
    return pool->pool[i];
   }
  }
 }
 else {
  pool->num_seg++;
  if (realloc(pool->pool,pool->num_seg*sizeof(void**))==NULL) {
   return NULL;
  }
  realloc(pool->status,pool->num_seg*sizeof(short));
  pool->pool[pool->num_seg-1]=malloc(pool->segsize);
  pool->status[pool->num_seg-1]=1;
  pool->num_used++;
  return pool->pool[pool->num_seg-1];
 }
 return NULL;
}

void XMLDMemPool_unget_segment(struct XMLDMemPool *pool, void *segment) {
 for (i=0;i<pool->num_seg;i++) {
  if (pool->pool[i] == segment) {
   pool->status[i]=0;
   pool->num_used--;
   break;
  }
 }
}
