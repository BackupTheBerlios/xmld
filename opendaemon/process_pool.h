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
 
#ifndef __PROCESS_POOL_H
#define __PROCESS_POOL_H

struct proc_pool;
struct proc {
 struct proc_pool *pool;
 void (*func) (void*);
 void *data;
 Bool busy;
 Bool die;
 pid_t pid;
};

struct proc_pool {
 int num;
 int num_busy;
 int init_proc;
 int max_proc;
 int max_idle_proc;
 struct proc *children;
};

typedef struct proc proc;
typedef struct proc_pool proc_pool;

proc_pool *process_pool_create(int, int, int);
Status process_pool_destroy(proc_pool *);
Status process_pool_handle(proc_pool *, void (*) (void*), void *, int);
proc *_pspawn(proc_pool *);
void _pexecutor(proc *);
void process_pool_kill_me(proc_pool *);

#endif /* __PROCESS_POOL_H */
