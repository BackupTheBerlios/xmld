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

Assoc *interface_list;
AssocWalker *interface_list_walker;

Status interface_list_init();
Status interface_list_shutdown();
Interface *interface_list_search_by_port(int);

#endif /* __INTREFACE_LIST_H */
