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
};

struct proc {
 void (*func) (void*);
 void *data;
 int busy;
 int die;
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

short mtasker_init(void);
short mtasker_shutdown(void);
short mtasker_handle(void (*) (void*), void*);
struct proc *mtasker_spawn();
short mtasker_kill(int);
void mtasker_handle_idle(int);

#endif /* __PTASKER_H */
