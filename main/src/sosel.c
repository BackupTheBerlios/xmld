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
#include "qp.h"
/*
 * Organize XMLDConnection removal (somanager_remove and sosel_shutdown) so
 * they don't cause segfaults or conflicts.
 */
int max_conn;
struct connection_table *conn_table;

xmld_status_t sosel_init() {
 max_conn=cfg_get("sosel.max_conn");
 /* FIXME: should check ftok's usage */
 key_t key=ftok("cfg.h", 'X');
 int id=shmget(key, sizeof(struct connection_table), IPC_CREAT);
 conn_table=(struct connection_table*) shmat(id, 0, 0);
 shmctl(id, IPC_RMID, 0);
 conn_table->used=0;
 key=ftok("errors.h", 'X');
 id=shmget(key, max_conn*sizeof(struct XMLDConnection), IPC_CREAT);
 conn_table->conn=(struct XMLDConnection*)shmat(id, 0, 0); 
 shmctl(id, IPC_RMID, 0);
 xmld_status_t stat=mtasker_handle(sosel_run, (void *) 0);
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
  conn_table->conn[i].sfd=0;
  conn_table->conn[i].fd=0;
  if (conn_table->conn[i].curr_dir!=0) {
   free(conn_table->conn[i].curr_dir);
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
 
 while (j!=0) {
  FD_ZERO(&fds);
  j=0;
  timeout.tv_sec=0;
  timeout.tv_usec=100; /* FIXME: this value should depend on cfg */
  for (i=0;i<max_conn;i++) {
   if (conn_table->conn[i].sfd==1) {
    FD_SET(conn_table->conn[i].fd, &fds);
    conns[j]=&conn_table->conn[i];
    j++;
    if (conn_table->conn[i].fd>maxfd) {
     maxfd=conn_table->conn[i].fd;
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
     mtasker_handle(qp_handle, (void*) conns[i]);
    }
   }
  }
 }
 free(conns);
}
xmld_status_t sosel_sremove(int fd) {
 int i;
 xmld_status_t stat=XMLD_FAILURE;
 for (i=0;i<max_conn;i++) {
  if (conn_table->conn[i].fd==fd) {
   conn_table->conn[i].sfd=0;
   stat=XMLD_SUCCESS;
   break;
  }
 }
 return stat;
}
xmld_status_t sosel_sadd(int fd) {
 int i;
 xmld_status_t stat=XMLD_FAILURE;
 for (i=0;i<max_conn;i++) {
  if (conn_table->conn[i].fd==fd) {
   conn_table->conn[i].sfd=1;
   stat=XMLD_SUCCESS;
   break;
  }
 }
 return stat;
}
xmld_status_t sosel_add(int fd, char*dir) {
 xmld_status_t stat=XMLD_FAILURE;
 int j;
 while (conn_table->used >= max_conn) {
 }
 for (j=0;j<max_conn;j++) {
   if (conn_table->conn[j].fd==0 && conn_table->conn[j].sfd==0) {
      conn_table->used++;
      conn_table->conn[j].curr_dir=(char*) malloc(strlen(dir)*sizeof(char));
      strcpy(conn_table->conn[j].curr_dir, dir);
      conn_table->conn[j].fd=fd;
      conn_table->conn[j].sfd=1;
      stat=XMLD_SUCCESS;
      break;
     }
  }
 return stat;
}
xmld_status_t sosel_remove(struct XMLDConnection *conn) {
 if (conn->curr_dir) {
  free(conn->curr_dir);
 } 
 conn->fd=0;
 conn->sfd=0;
 conn_table->used--;
 return XMLD_SUCCESS;
}
