#include <stdlib.h>
#include "xmld_mempool.h"

int i;
struct XMLDMemPool *XMLDMemPool_create(int segsize, int num_seg) {
 struct XMLDMemPool *pool=(struct XMLDMemPool *)malloc(sizeof(struct XMLDMemPool));
 pool->num_seg=num_seg;
 pool->segsize=segsize;
 pool->pool=(void**) malloc(num_seg*sizeof(void*));
 for (num_seg=0; num_seg < pool->num_seg; num_seg++) {
  pool->pool[num_seg]=calloc(1,segsize);
 }
 pool->num_used=0;
 return pool;
}

void XMLDMemPool_free(struct XMLDMemPool *pool) {
 for (i=0;i<pool->num_seg;i++) {
  free(pool->pool[i]);
 }
 free(pool->pool);
 free(pool);
}

void *XMLDMemPool_get_segment(struct XMLDMemPool *pool) {
 if (pool->num_used < pool->num_seg) {
  for (i=0;i< pool->num_seg ;i++) {
   if (((int*) (pool->pool[i]))[0]==0) {
    pool->num_used++;
    ((int*) (pool->pool[i]))[0]=1;
    return pool->pool[i];
   }
  }
 }
 else {
  pool->num_seg++;
  if (realloc(pool->pool,pool->num_seg*sizeof(void**))==NULL) {
   return NULL;
  }
  pool->pool[pool->num_seg-1]=calloc(1, pool->segsize);
  pool->num_used++;
  ((int*) (pool->pool[pool->num_seg-1]))[0]=1;
  return pool->pool[pool->num_seg-1];
 }
 return NULL;
}

void XMLDMemPool_free_segment(struct XMLDMemPool *pool, void *segment) {
 for (i=0;i<pool->num_seg;i++) {
  if (pool->pool[i] == segment) {
   ((int*) (pool->pool[i]))[0]=0;
   pool->num_used--;
   break;
  }
 }
}
