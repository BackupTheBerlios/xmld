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
 
#ifndef HAVE_SOMANAGER_H
#define HAVE_SOMANAGER_H

xmld_status_t somanager_init();
xmld_status_t somanager_shutdown();
void somanager_handle(void*);

#endif /* HAVE_SOMANAGER_H */
