#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "errors.h"
#include "cfg.h"
#include "xmld_types.h"
#include "sosel.h"
#include "mtasker.h"

/*
 * Organize XMLDConnection removal (somanager_remove and sosel_shutdown) so
 * they don't cause segfaults or conflicts.
 */
xmld_status_t stat;
int max_conn;
struct XMLDConnection *conn_table;
int j;

xmld_status_t sosel_init() {
 max_conn=cfg_get("sosel.max_conn");
 key_t key=ftok("errors.h", 'X');
 int id=shmget(key, max_conn*sizeof(struct XMLDConnection), IPC_CREAT);
 conn_table=(struct XMLDConnection*)shmat(id, 0, 0); 
 shmctl(id, IPC_RMID, 0);
 void *k;
 stat=mtasker_handle(sosel_run, k);
 if (stat!=XMLD_SUCCESS) {
  return XMLD_FAILURE;
 }
 else {
  return XMLD_SUCCESS;
 }
}
xmld_status_t sosel_shutdown() {
 int i;
 for (i=0;i<max_conn;i++) {
  conn_table[i].sfd=0;
  conn_table[i].fd=0;
  if (conn_table[i].curr_dir!=0) {
   free(conn_table[i].curr_dir);
  } 
 }
 shmdt((void *)conn_table);
 return XMLD_SUCCESS;
}
void sosel_run(void *data) {
 fd_set fds;
 struct timeval timeout;
 struct XMLDConnection **conns=(struct XMLDConnection **)malloc(max_conn*sizeof(struct XMLDConnection*));
 int maxfd=0,ret,i,j=1;
 j=1;
 int maxfd;
 
 while (j!=0) {
  FD_ZERO(&fds);
  j=0;
  timeout.tv_sec=0;
  timeout.tv_usec=100; /* FIXME: this value should depend on cfg */
  for (i=0;i<max_conn;i++) {
   if (conn_table[i].sfd==1) {
    FD_SET(conn_table[i].fd, &fds);
    conns[j]=&conn_table[i];
    j++;
    if (conn_table[i].fd>maxfd) {
     maxfd=conn_table[i].fd;
    }
   }
  }
  ret=select(maxfd, &fds, NULL, NULL, &timeout);
  if (ret==0) {
   continue;
  }
  else if (ret==-1) {
   perror("select");
   break;
  }
  else {
   for (i=0;i<=j;i++) {
    if (FD_ISSET(conns[i]->fd, &fds)) {
     conns[i]->sfd=0;
     mtasker_handle(qparser_handle, (void*) conns[i]);
    }
   }
  }
 }
 free(conns);
}
xmld_status_t sosel_sremove(int fd) {
 int i;
 stat=XMLD_FAILURE;
 for (i=0;i<max_conn;i++) {
  if (conn_table[i].fd==fd) {
   conn_table[i].sfd=0;
   stat=XMLD_SUCCESS;
   break;
  }
 }
 return stat;
}
xmld_status_t sosel_sadd(int fd) {
 int i;
 stat=XMLD_FAILURE;
 for (i=0;i<max_conn;i++) {
  if (conn_table[i].fd==fd) {
   conn_table[i].sfd=1;
   stat=XMLD_SUCCESS;
   break;
  }
 }
 return stat;
}
xmld_status_t sosel_add(int fd, char*dir) {
 stat=XMLD_FAILURE;
 for (j=0;j<max_conn;j++) {
   if (conn_table[j].fd==0 && conn_table[j].sfd==0) {
      conn_table[j].curr_dir=(char*)malloc(strlen(dir)*sizeof(char));
      strcpy(conn_table[j].curr_dir, dir);
      conn_table[j].fd=fd;
      conn_table[j].sfd=1;
      stat=XMLD_SUCCESS;
      break;
     }
  }
 return stat;
}
xmld_status_t sosel_remove(int fd) {
 stat=XMLD_FAILURE;
 for (j=0;j<max_conn;j++) {
  if (conn_table[j].fd==fd) {
   free(conn_table[j].curr_dir);
   conn_table[j].fd=0;
   conn_table[j].sfd=0;
   stat=XMLD_SUCCESS;
   break;
  }
 }
 return stat;
}
