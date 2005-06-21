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
 
#ifndef __INTERFACE_LIST_H
#define __INTERFACE_LIST_H

XMLDAssoc *interface_list;
XMLDAssocWalker *interface_list_walker;

XMLDStatus interface_list_init();
XMLDStatus interface_list_shutdown();

#endif /* __INTREFACE_LIST_H */
