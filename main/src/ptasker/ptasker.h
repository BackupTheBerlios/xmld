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
 
#ifndef __PTASKER_H
#define __PTASKER_H

struct task {
 void (*func) (void*);
 void *data;
 int fd;
};

struct proc {
 void (*func) (void*);
 void *data;
 int busy;
 int die;
 int fd;
 int sp[2];
 pid_t pid;
};

struct proc_table {
 int num;
 int num_busy;
 struct proc *children;
};

struct task_table {
 struct task *tasks;
 int num_tasks;
};

int passed_fd; /*
		* The fd last passed using passfd.c
		*/

int mtasker_init(void);
int mtasker_shutdown(void);
int mtasker_handle(void (*) (void*), void *, int);
struct proc *mtasker_spawn();
void mtasker_kill(int);
void mtasker_handle_idle(int);

#endif /* __PTASKER_H */
