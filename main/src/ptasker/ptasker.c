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
#include "../cfg.h"
#include "../passfd.h"
#include "ptasker.h"

/*
 * FIXME: secure mtasker_handle_idle (SIGUSR1)
 */
struct proc_table *table;
struct proc *curr_proc;
struct task_table ttable;
pid_t curr_pid;
int init_proc;
int max_proc;
int max_idle_proc;

short mtasker_init() {
 int i;
 struct sigaction action;
 action.sa_handler=mtasker_handle_idle;
 
 i=sigaction(SIGUSR1, &action, NULL);
 if (i == -1) {
  return 0;
 }
 
 signal(SIGCHLD, SIG_IGN);
 ttable.num_tasks=0;
 ttable.tasks=0;
 
 init_proc=*((int *) cfg_get("mtasker.init_proc"));
 max_proc=*((int *) cfg_get("mtasker.max_proc"));
 max_idle_proc=*((int *) cfg_get("mtasker.max_idle_proc"));

 if (init_proc > max_proc) {
  init_proc=max_proc;
 }
 
 key_t tablekey=ftok("xmld.c",'X');
 key_t childrenkey=ftok("init.h",'X');
 
 int tableid=shmget(tablekey, sizeof(struct proc_table), IPC_CREAT);
 int childrenid=shmget(childrenkey, max_proc*sizeof(struct proc), IPC_CREAT);
 
 if (tableid == -1) {
  return 0;
 }
 
 table=(struct proc_table *)shmat(tableid, 0, 0);
 shmctl(tableid, IPC_RMID, 0);
 
 if (childrenid == -1) {
  return 0;
 }
 
 table->num=0;
 table->num_busy=0;
 table->children=(struct proc *)shmat(childrenid, 0,0);
 shmctl(childrenid, IPC_RMID, 0);
 for (i=0; i < init_proc; i++) {
  table->num++;
  table->children[i].pid=-1;
  table->children[i].fd=-1;
    
  if(socketpair(PF_UNIX, SOCK_STREAM, 0, table->children[i].sp) != 0) {
   table->children[i].pid=0;
   perror("mtasker_spawn");
   return 0; 
  }
  curr_pid=fork();

  if (curr_pid == -1) {
   table->children[i].pid=0;
   perror("mtasker_init");
   mtasker_shutdown();
   return 0;
  }
  else if (curr_pid==0) {
   table->children[i].pid=getpid();
   signal(SIGCHLD, SIG_IGN);
   
   while (1) {
    if (table->children[i].func != 0) {
     if (table->children[i].fd != -1) {
      passed_fd=recvfd(table->children[i].sp[1]);
     }
     (*(table->children[i].func))(table->children[i].data);
     table->children[i].busy=0;
     table->children[i].func=0;
     table->children[i].data=0;
     table->children[i].fd=-1;
     table->num_busy--;
     kill(getppid(), SIGUSR1);
    } 
    if (table->children[i].die==1) {
     table->children[i].pid=0;
     table->num--;
     table->children[i].die=0;
     break;
    }
   }
   exit(0);
  }
 }
 return 1;
}
short mtasker_shutdown() {
 int i;
 for (i = 0; i < max_proc; i++) {
  shutdown(table->children[i].sp[0], 2);
  shutdown(table->children[i].sp[1], 2);
  if (table->children[i].pid != 0) {
   kill(table->children[i].pid, SIGTERM);
  } 
  table->children[i].die=1;
 }
 free(ttable.tasks);
 return 1;
}
short mtasker_handle(void (*func) (void *), void *data, int fd) {
 int i;
 struct proc *use_proc;
 if (table->num > table->num_busy) {
  for (i=0;i<max_proc;i++) {
   if (table->children[i].busy == 0 && table->children[i].pid != 0) {
    use_proc=&table->children[i];
    break;
   }
  }
 }
 else if (table->num == table->num_busy) {
  if (table->num < max_proc) {
   use_proc=mtasker_spawn();
  }
  else {
   ttable.num_tasks++;
   ttable.tasks=realloc(ttable.tasks, ttable.num_tasks*sizeof(struct task));
   ttable.tasks[ttable.num_tasks-1].func=func;
   ttable.tasks[ttable.num_tasks-1].data=data;
   ttable.tasks[ttable.num_tasks-1].fd=fd;
   return 1;
  }
 }

 if (use_proc!=0) {
  use_proc->func=func;
  use_proc->data=data;
  if (fd != -1 && sendfd(use_proc->sp[0], fd)) {
   use_proc->fd=fd;
  } 
  use_proc->busy=1;
  table->num_busy++;
  return 1;
 }
 else {
  return 0;
 }
}

struct proc *mtasker_spawn() {
 int i;
 if (table->num > max_proc) {
  return 0;
 }
 else {
  for (i = 0; i < max_proc; i++) {
   if (table->children[i].pid==0) {
    table->num++;
    table->children[i].pid=-1;
    table->children[i].fd=-1;
    
    if(socketpair(PF_UNIX, SOCK_STREAM, 0, table->children[i].sp) != 0) {
     table->children[i].pid=0;
     perror("mtasker_spawn");
     return 0; 
    }
    curr_pid=fork();
    
    if (curr_pid==-1) {
     table->children[i].pid=0;
     perror("mtasker_spawn");
     return 0;
    }
    else if (curr_pid==0) {
     table->children[i].pid=getpid();
     signal(SIGCHLD, SIG_IGN);
     
     while (1) {
      if (table->children[i].func!=0) {
       if (table->children[i].fd != -1) {
        passed_fd=recvfd(table->children[i].sp[1]);
       }
       (*(table->children[i].func))(table->children[i].data);
       table->children[i].busy=0;
       table->children[i].func=0;
       table->children[i].data=0;
       table->children[i].fd=-1;
       table->num_busy--;
       kill(getppid(), SIGUSR1);
      } 
      if (table->children[i].die==1) {
       table->children[i].pid=0;
       table->children[i].die=0;
       table->num--;
       break;
      }
     }
     exit(0);
    }
    break;    
   }
  }
  return &table->children[i];
 }
}

short mtasker_kill(int num_proc) {
 int i;
 int killed_proc=0;
 for (i=0;i<max_proc;i++) {
  if (table->children[i].busy != 1 && table->children[i].pid != 0) {
   table->children[i].die=1;
   if (++killed_proc >= num_proc) {
    break;
   }
  }  
 }
 return 0;
}

void mtasker_handle_idle(int signum) {
 int j;
 short status;
 if (ttable.num_tasks>0) {
  status=mtasker_handle(ttable.tasks[0].func, ttable.tasks[0].data, ttable.tasks[0].fd);
  if (status!=0) {
   return;
  }
  for (j = 0; j < ttable.num_tasks-1; j++) {
   ttable.tasks[j]=ttable.tasks[j+1];
  }
  ttable.tasks[ttable.num_tasks-1].func=0;
  ttable.tasks[ttable.num_tasks-1].data=0;
  ttable.tasks[ttable.num_tasks-1].fd=-1;
  ttable.num_tasks--;
  ttable.tasks=realloc(ttable.tasks, ttable.num_tasks*sizeof(struct task));
 }
 if (table->num - table->num_busy > max_idle_proc) {
  mtasker_kill((table->num - table->num_busy) - max_idle_proc);
 }
}
