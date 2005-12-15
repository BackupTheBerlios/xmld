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
#include "ptasker.h"

proc_pool *ptasker_create_pool(int init_proc, int max_proc, int max_idle_proc, char *tok) {
 key_t tablekey=ftok(tok ,'X');
 key_t childrenkey=ftok(tok ,'X');
 
 if (init_proc > max_proc) {
  init_proc = max_proc;
 }
 
 int tableid = shmget(tablekey, sizeof(proc_pool), IPC_CREAT);
 int childrenid = shmget(childrenkey, max_proc*sizeof(proc), IPC_CREAT);
 
 if (tableid == -1) {
  return NULL;
 }
 
 table = (proc_table *) shmat(tableid, 0, 0);
 shmctl(tableid, IPC_RMID, 0);
 
 if (childrenid == -1) {
  return NULL;
 }
 
 table->num=0;
 table->num_busy=0;
 table->init_proc = init_proc;
 table->max_proc = max_proc;
 table->max_idle_proc = max_idle_proc;
 table->children=(struct proc *)shmat(childrenid, 0,0);
 shmctl(childrenid, IPC_RMID, 0);
 int curr_pid;
 
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
   ptasker_destroy_pool(table);
   return NULL;
  }
  else if (curr_pid == 0) {
   _pexecutor(table->children + i);
  }
 }
 return table;
}

Status ptasker_destroy_pool(proc_pool *pool) {
 int i;
 for (i = 0; i < pool->max_proc; i++) {
  pool->children[i].die = TRUE;
 }

 while (pool->num_busy > 0) {
 }

 shmdt(pool->children);
 shmdt(pool);
 return SUCCESS;
}

Status ptasker_handle(proc_pool *pool, void (*func) (void *), void *data) {
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
   use_proc = _pspawn(pool);
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
    
 int curr_pid=fork();
    
 if (curr_pid == -1) {
  pool->children[i].pid = 0;
  return NULL;
 }
 else if (curr_pid == 0) {
  _pexecutor(pool->children + i);
 }

 return pool->children + i;
}

void _pexecutor(proc *proc_data) {
 proc->pid = getpid();

 while (1) {
  if (proc->func != NULL) {
   proc->busy = TRUE;
   proc->pool->num_busy++;
   (*(proc->func)) (proc->data);
   proc->busy = FALSE;
   proc->func = NULL;
   proc->data = NULL;
   proc->pool->num_busy--;
  }
    
  if (proc->pool->num - proc->pool->num_busy > proc->pool->max_idle_proc) {
   proc->pid = 0;
   proc->die = FALSE;
   proc->pool->num--;
   break;
  }
    
  if (proc->die == TRUE) {
   proc->pid = 0;
   proc->die = FALSE;
   proc->pool->num--;
   break;
  }
 }
 exit(0);
}
