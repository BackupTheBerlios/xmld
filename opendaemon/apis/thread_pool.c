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

#include "thread_pool.h"

thread_pool *thread_pool_create(int init_threads, int max_threads, int max_idle_threads) {
 thread_pool *pool = (thread_pool *) malloc(sizeof(thread_pool));
 pool->num = 0;
 pool->num_busy = 0;
 pool->max_threads = max_threads;
 pool->max_idle_threads = max_idle_threads;
 pool->children = (thread *) malloc(sizeof(thread) * max_threads);
 
 if (init_threads > max_threads) {
  init_threads = max_threads;
 }

 int i;
 for (i = 0; i < max_threads; i++) {
  pool->children[i].thread_id = 0;
 }

 for (i = 0; i < init_threads; i++) {
  if (_tspawn(pool, i) == NULL) {
   thread_pool_destroy(pool);
   return NULL;
  }
 }

 return pool;
}

Status thread_pool_handle(thread_pool *pool, void (*func) (void *), void *data) {
 thread *free_thread = NULL;
 
 if (pool->num_busy == pool->num) {
  if (pool->num < pool->max_threads) {
   free_thread = _tspawn(pool, -1);
  }
  else {
   while (pool->num_busy == pool->num) {
   }
  }
 }
 
 int i;
 for (i = 0; i < pool->max_threads; i++) {
  if (pool->children[i].busy == FALSE && pool->children[i].thread_id != 0) {
   free_thread = pool->children + i;
   break;
  }		  
 }
 
 free_thread->func = func;
 free_thread->data = data;

 return SUCCESS;
}

Status thread_pool_destroy(thread_pool *pool) {
 int i;
 for (i = 0; i < pool->max_threads; i++) {
  pool->children[i].die = TRUE;
 }
 
 while (pool->num_busy > 0) {
 }

 free(pool->children);
 free(pool);
 return SUCCESS;
}

void _texecutor(void *data) {
 thread_data = (thread *) data;
 while (1) {
  if (thread_data->func != NULL) {
   thread_data->busy = TRUE;
   thread_data->pool->num_busy++;
   *(thread_data->func) (thread_data->data);
   thread_data->func = NULL;
   thread_data->data = NULL;
   thread_data->busy = FALSE;
   thread_data->pool->num_busy--;
   
   if (thread_data->pool->num - thread_data->pool->num_busy > thread_data->pool->max_idle_threads) {
    thread_data->die = FALSE;
    thread_data->thread_id = 0;
    thread_data->pool->num--;
    pthread_exit(NULL);
   }
   
   if (thread_data->die == TRUE) {
    thread_data->die = FALSE;
    thread_data->thread_id = 0;
    thread_data->pool->num--;
    pthread_exit(NULL);
   }
  }
 }
}

thread *_tspawn(thread_pool *pool, int index) {
 
 if (index == -1) {
  int i;
  for (i = 0; i < pool->max_threads; i++) {
   if (pool->children[i].thread_id == 0) {
    index = i;
    break;
   }
  }
 }
 
 if (index == -1) {
  return NULL;
 }
 
 if (pthread_create(&pool->children[index].thread_id, NULL, _texecutor, (void *) (pool->children + index)) == 0) {
  pool->children[index].busy = FALSE;
  pool->children[index].die = FALSE;
  pool->children[index].func = NULL;
  pool->children[index].data = NULL;
  pool->children[index].pool = pool;
  pool->num++;
  return pool->children + index;
 }
 else {
  pool->children[index].thread_id = 0;
  return NULL;
 }
}
