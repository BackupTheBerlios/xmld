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
 
#ifndef HAVE_INIT_H
#define HAVE_INIT_H

/* A structure representing a part that needs initialization */
struct xmld_part {
 short (*init_func) (void); /* initialization function */
 short (*shutdown_func) (void); /* shutdown function */
 int ok; /* whether initilization went fine */
};

void init_create_part(struct xmld_part*, short (*) (void), short (*) (void));
void init_shutdown_parts(void);
#endif /* HAVE_INIT_H */

