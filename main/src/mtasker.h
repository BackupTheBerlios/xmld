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
 
#ifndef HAVE_MTASKER_H
#define HAVE_MTASKER_H

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

xmld_status_t mtasker_init(void);
xmld_status_t mtasker_shutdown(void);
xmld_status_t mtasker_handle(void (*) (void*), void*);
struct proc *mtasker_spawn();
xmld_status_t mtasker_kill(int);
void mtasker_handle_idle(int);
#endif /* HAVE_MTASKER_H */
