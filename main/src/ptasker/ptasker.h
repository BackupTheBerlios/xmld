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

struct proc {
 void (*func) (void*);
 void *data;
 XMLDBool busy;
 XMLDBool die;
 int fd;
 int sp[2];
 pid_t pid;
};

struct proc_table {
 int num;
 int num_busy;
 struct proc *children;
};

int passed_fd; /*
		* The fd last passed using passfd.c
		*/

XMLDStatus mtasker_init(void);
XMLDStatus mtasker_shutdown(void);
XMLDStatus mtasker_handle(void (*) (void*), void *, int);
struct proc *mtasker_spawn();
void mtasker_kill(int);
void mtasker_handle_idle(int);
void mtasker_signal_children(int);

#endif /* __PTASKER_H */
