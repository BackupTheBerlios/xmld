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
 
#ifndef __SOMANAGER_H
#define __SOMANAGER_H

XMLDStatus somanager_init();
XMLDStatus somanager_shutdown();
void somanager_handle(void*);
void somanager_user_connection(void*);

#endif /* __SOMANAGER_H */
