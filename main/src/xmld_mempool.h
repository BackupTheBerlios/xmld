#ifndef HAVE_XMLD_MEMPOOL_H
#define HAVE_XMLD_MEMPOOL_H

struct XMLDMemPool {
 void **pool;
 int num_seg;  /* the number of allocated segments */
 int num_used; /* the number of used segments */
 int segsize;  /* size of each segment */
};

struct XMLDMemPool *XMLDMemPool_create(int,int);
void XMLDMemPool_free(struct XMLDMemPool*);
void *XMLDMemPool_get_segment(struct XMLDMemPool *);
void XMLDMemPool_free_segment(struct XMLDMemPool *pool, void*);
#endif /* HAVE_XMLD_MEMPOOL_H */
