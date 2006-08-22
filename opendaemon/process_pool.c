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
 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "includes.h"
#include "process_pool.h"

proc_pool *process_pool_create(int init_proc, int max_proc, int max_idle_proc) {
 static int proj_id = 1;
 
 key_t tablekey = ftok("opendaemon.c", proj_id);
 proj_id++;
 key_t childrenkey = ftok("opendaemon.c" , proj_id);
 
 if (init_proc > max_proc) {
  init_proc = max_proc;
 }
 
 int tableid = shmget(tablekey, sizeof(proc_pool), IPC_CREAT);
 int childrenid = shmget(childrenkey, max_proc*sizeof(proc), IPC_CREAT);
 
 if (tableid == -1) {
  return NULL;
 }
 
 proc_pool *table = (proc_pool *) shmat(tableid, 0, 0);
 shmctl(tableid, IPC_RMID, 0);
 
 if (childrenid == -1) {
  return NULL;
 }
 
 table->num=0;
 table->num_busy=0;
 table->init_proc = init_proc;
 table->max_proc = max_proc;
 table->max_idle_proc = max_idle_proc;
 table->childrenid = childrenid;
 table->tableid = tableid;
 table->children=(proc *) shmat(childrenid, 0, 0);
 shmctl(childrenid, IPC_RMID, 0);
 int curr_pid;

 int i;
 for (i=0; i < init_proc; i++) {
  table->num++;
  table->children[i].pid = 0;
  table->children[i].func=NULL;
  table->children[i].data=NULL;
  table->children[i].busy=FALSE;
  table->children[i].die=FALSE;
  
  curr_pid=fork();
  
  if (curr_pid == -1) {
   table->children[i].pid=0;
   process_pool_destroy(table);
   return NULL;
  }
  else if (curr_pid == 0) {
   table = (proc_pool *) shmat(tableid, 0, 0);
   table->children = (proc *) shmat(childrenid, 0, 0);
   table->children[i].pool = table;
   _pexecutor(table->children + i);
  }
 }
 return table;
}

Status process_pool_destroy(proc_pool *pool) {
 int i;
 
 //FIXME: why does pool->children need shmat again ?
 pool->children = (proc *) shmat(pool->childrenid, 0, 0);

 for (i = 0; i < pool->max_proc; i++) {
  pool->children[i].die = TRUE;
 }

 while (pool->num_busy > 0) {
 }

 shmdt(pool->children);
 shmdt(pool);
 return SUCCESS;
}

Status process_pool_handle(proc_pool *pool, void (*func) (void *), void *data) {
 int i;
 proc *use_proc = NULL;

 while (pool->num == pool->num_busy && pool->num >= pool->max_proc) {
 }
 
 if (pool->num > pool->num_busy) {
  for (i = 0; i < pool->max_proc; i++) {
   if (pool->children[i].busy == FALSE && pool->children[i].pid != 0) {
    use_proc = pool->children + i;
    break;
   }
  }
 }
 else if (pool->num == pool->num_busy) {
  if (pool->num < pool->max_proc) {
   use_proc = _pspawn(pool, -1);
  }
 }

 if (use_proc != NULL) {
  use_proc->func = func;
  use_proc->data = data;
  use_proc->busy = TRUE;
  pool->num_busy++;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

proc *_pspawn(proc_pool *pool, int index) {
 
 if (index == -1) {
  int i;
  for (i = 0; i < pool->max_proc; i++) {
   if (pool->children[i].busy == FALSE && pool->children[i].pid == 0) {
    index = i;
    break;
   }
  }
 }

 if (index == -1) {
  return NULL;
 }
 
 pool->num++;
 pool->children[index].pid = 0;
 pool->children[index].func = NULL;
 pool->children[index].data = NULL;
 pool->children[index].busy = FALSE;
 pool->children[index].die = FALSE;
 int tableid = pool->tableid;
 int childrenid = pool->childrenid;

 int curr_pid=fork();
    
 if (curr_pid == -1) {
  pool->children[index].pid = 0;
  return NULL;
 }
 else if (curr_pid == 0) {
  pool = (proc_pool *) shmat(tableid, 0, 0);
  pool->children = (proc *) shmat(childrenid, 0, 0);
  pool->children[index].pool = pool;
  _pexecutor(pool->children + index);
 }

 return (pool->children + index);
}

void _pexecutor(proc *proc_data) {
 
 proc_data->pid = getpid();
 
 while (1) {
  if (proc_data->func != NULL) {
   proc_data->busy = TRUE;
   proc_data->pool->num_busy++;
    printf("dd");
   (*(proc_data->func)) (proc_data->data);
   proc_data->busy = FALSE;
   proc_data->func = NULL;
   proc_data->data = NULL;
   proc_data->pool->num_busy--;
  }
    
  if (proc_data->pool->num - proc_data->pool->num_busy > proc_data->pool->max_idle_proc) {
   proc_data->pid = 0;
   proc_data->die = FALSE;
   proc_data->pool->num--;
   break;
  }
    
  if (proc_data->die == TRUE) {
   proc_data->pid = 0;
   proc_data->die = FALSE;
   proc_data->pool->num--;
   break;
  }
  usleep(20000);
 }

 exit(0);
}

void process_pool_kill_me(proc_pool *pool) {
 int i;
 for (i = 0; i < pool->max_proc; i++) {
  if (pool->children[i].pid == getpid()) {
   pool->children[i].die = TRUE;
   break;
  }
 }
}
