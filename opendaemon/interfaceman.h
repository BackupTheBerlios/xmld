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

#ifndef __INTERFACEMAN_H
#define __INTERFACEMAN_H

#define MTYPE_CODE 1
#define MTYPE_CONF 2
#define MTYPE_USER 3
#define MAX_MSG_LEN 50

struct msgbuf {
 long mtype;
 char mtext[50];
};

Status interfaceman_init(void);
void _launch(void *);

#endif /* __INTERFACEMAN_H */
