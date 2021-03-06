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
#include "../includes.h"
#include "../passfd.h"
#include "ptasker.h"

/*
 * FIXME: secure mtasker_handle_idle (SIGUSR1)
 */
struct proc_table *table;
struct proc *curr_proc;
pid_t curr_pid;
int init_proc;
int max_proc;
int max_idle_proc;

Status mtasker_init() {
 int i;
 struct sigaction action;
 action.sa_handler=mtasker_handle_idle;
 
 i=sigaction(SIGUSR1, &action, NULL);
 if (i == -1) {
  return FAILURE;
 }
 
 signal(SIGCHLD, SIG_IGN);
 CfgSection *ptasker_section = CfgSection_get_section(cfg_tree, "PTasker", 0);
 if (ptasker_section == NULL) {
  return FAILURE;
 }
 CfgDirective *curr_directive = CfgSection_get_directive(ptasker_section, "InitProc", 0);
 if (curr_directive == NULL) {
  return FAILURE;
 }
 CfgValue *curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  return FAILURE;
 }
 init_proc = (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxProc", 0);
 if (curr_directive == NULL) {
  return FAILURE;
 }
 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  return FAILURE;
 }
 max_proc= (int) curr_value->value;
 curr_directive = CfgSection_get_directive(ptasker_section, "MaxIdleProc", 0);
 if (curr_directive == NULL) {
  return FAILURE;
 }
 curr_value = CfgDirective_get_value(curr_directive, 0);
 if (curr_value == NULL || curr_value->type != CFG_INTEGER) {
  return FAILURE;
 }
 max_idle_proc= (int) curr_value->value;

 if (init_proc > max_proc) {
  init_proc = max_proc;
 }
 
 key_t tablekey=ftok("xmld.c",'X');
 key_t childrenkey=ftok("init.h",'X');
 
 int tableid=shmget(tablekey, sizeof(struct proc_table), IPC_CREAT);
 int childrenid=shmget(childrenkey, max_proc*sizeof(struct proc), IPC_CREAT);
 
 if (tableid == -1) {
  return FAILURE;
 }
 
 table=(struct proc_table *)shmat(tableid, 0, 0);
 shmctl(tableid, IPC_RMID, 0);
 
 if (childrenid == -1) {
  return FAILURE;
 }
 
 table->num=0;
 table->num_busy=0;
 table->children=(struct proc *)shmat(childrenid, 0,0);
 shmctl(childrenid, IPC_RMID, 0);
 for (i=0; i < init_proc; i++) {
  table->num++;
  table->children[i].pid=-1;
  table->children[i].fd=-1;
  table->children[i].func=NULL;
  table->children[i].data=NULL;
  table->children[i].busy=FALSE;
  table->children[i].die=FALSE;
      
  if(socketpair(PF_UNIX, SOCK_STREAM, 0, table->children[i].sp) != 0) {
   table->children[i].pid=0;
   perror("mtasker_spawn");
   return FAILURE; 
  }
  curr_pid=fork();

  if (curr_pid == -1) {
   table->children[i].pid=0;
   perror("mtasker_init");
   mtasker_shutdown();
   return FAILURE;
  }
  else if (curr_pid == 0) {
   table->children[i].pid=getpid();
   signal(SIGCHLD, SIG_IGN);
   signal(SIGUSR2, cfg_update);
   
   while (1) {
    if (table->children[i].func != NULL) {
     if (table->children[i].fd != -1) {
      passed_fd=recvfd(table->children[i].sp[1]);
     }
     (*(table->children[i].func))(table->children[i].data);
     table->children[i].busy=FALSE;
     table->children[i].func=NULL;
     table->children[i].data=NULL;
     table->children[i].fd=-1;
     table->num_busy--;
     kill(getppid(), SIGUSR1);
    } 
    if (table->children[i].die == TRUE) {
     table->children[i].pid=0;
     table->num--;
     table->children[i].die=FALSE;
     break;
    }
   }
   exit(0);
  }
 }
 return SUCCESS;
}
Status mtasker_shutdown() {
 int i;
 for (i = 0; i < max_proc; i++) {
  shutdown(table->children[i].sp[0], 2);
  shutdown(table->children[i].sp[1], 2);
  if (table->children[i].pid != 0) {
   kill(table->children[i].pid, SIGTERM);
  } 
  table->children[i].die = TRUE;
 }
 return SUCCESS;
}
Status mtasker_handle(void (*func) (void *), void *data, int fd) {
 int i;
 struct proc *use_proc;
 while (table->num == table->num_busy && table->num >= max_proc) {
 }
 if (table->num > table->num_busy) {
  for (i = 0; i < max_proc; i++) {
   if (table->children[i].busy == FALSE && table->children[i].pid != 0) {
    use_proc=&table->children[i];
    break;
   }
  }
 }
 else if (table->num == table->num_busy) {
  use_proc=mtasker_spawn();
 }

 if (use_proc != 0) {
  use_proc->func=func;
  use_proc->data=data;
  if (fd != -1 && sendfd(use_proc->sp[0], fd)) {
   use_proc->fd=fd;
  } 
  use_proc->busy = TRUE;
  table->num_busy++;
  return SUCCESS;
 }
 else {
  return FAILURE;
 }
}

struct proc *mtasker_spawn() {
 int i;
 if (table->num >= max_proc) {
  return NULL;
 }
 else {
  for (i = 0; i < max_proc; i++) {
   if (table->children[i].pid==0) {
    table->num++;
    table->children[i].pid=-1;
    table->children[i].fd=-1;
    table->children[i].func=NULL;
    table->children[i].data=NULL;
    table->children[i].busy=FALSE;
    table->children[i].die=FALSE;
    
    if(socketpair(PF_UNIX, SOCK_STREAM, 0, table->children[i].sp) != 0) {
     table->children[i].pid=0;
     perror("mtasker_spawn");
     return NULL; 
    }
    curr_pid=fork();
    
    if (curr_pid == -1) {
     table->children[i].pid=0;
     perror("mtasker_spawn");
     return NULL;
    }
    else if (curr_pid == 0) {
     table->children[i].pid=getpid();
     signal(SIGCHLD, SIG_IGN);
     signal(SIGUSR2, cfg_update);
     
     while (1) {
      if (table->children[i].func != NULL) {
       if (table->children[i].fd != -1) {
        passed_fd=recvfd(table->children[i].sp[1]);
       }
       (*(table->children[i].func))(table->children[i].data);
       table->children[i].busy=FALSE;
       table->children[i].func=NULL;
       table->children[i].data=NULL;
       table->children[i].fd=-1;
       table->num_busy--;
       kill(getppid(), SIGUSR1);
      } 
      if (table->children[i].die == TRUE) {
       table->children[i].pid=0;
       table->children[i].die=FALSE;
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

void mtasker_kill(int num_proc) {
 int i;
 int killed_proc=0;
 for (i = 0; i < max_proc; i++) {
  if (table->children[i].busy != TRUE && table->children[i].pid != 0) {
   table->children[i].die = TRUE;
   if (++killed_proc >= num_proc) {
    break;
   }
  }  
 }
}

void mtasker_handle_idle(int signum) {
 if (table->num - table->num_busy > max_idle_proc) {
  mtasker_kill((table->num - table->num_busy) - max_idle_proc);
 }
}

void mtasker_signal_children(int signum) {
 int i;
 for (i = 0; i < table->num; i++) {
  kill(table->children[i].pid, signum);
 }
}
