/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */


#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

struct thread_pool;

struct thread {
 thread_t thread_id;
 struct thread_pool *pool;
 void (*func) (void*);
 void *data;
 Bool busy;
 Bool die; 
};

struct thread_pool {
 int num;
 int num_busy;
 int max_threads;
 int max_idle_threads;
 struct thread *children;
};

typedef struct thread thread;
typedef struct thread_pool thread_pool;

thread_pool *thread_pool_create(int, int, int);
Status thread_pool_destroy(thread_pool *);
Status thread_pool_handle(thread_pool *, void (*) (void*), void *);
thread *_tspawn(thread_pool *);

#endif /* __THREAD_POOL_H */
