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
 
#ifndef HAVE_SOSEL_H
#define HAVE_SOSEL_H

short sosel_init(void);
short sosel_shutdown(void);
short sosel_add(int, char*);
short sosel_remove(XMLDConnection *);
void sosel_run(void*);

/* represents a connection table */
struct connection_table {
 short used; /* Number of used XMLDConnection's */
 XMLDConnection *conn;
};

#endif /* HAVE_SOSEL_H */
