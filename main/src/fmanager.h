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

#ifndef __FMANAGER_H
#define __FMANAGER_H

FILE *fmanager_get_sh_fd(char *);
FILE *fmanager_get_ex_fd(char *);
XMLDStatus fmanager_unlock_fd(FILE *);

#endif /* __FMANAGER_H */
