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
 
#ifndef __INIT_H
#define __INIT_H

/* A structure representing a part that needs initialization */
struct xmld_part {
 XMLDStatus (*init_func) (void); /* initialization function */
 XMLDStatus (*shutdown_func) (void); /* shutdown function */
 XMLDBool ok; /* whether initilization went fine */
};

void init_create_part(struct xmld_part *, XMLDStatus (*) (void), XMLDStatus (*) (void));
void init_shutdown_parts(int);
#endif /* __INIT_H */

