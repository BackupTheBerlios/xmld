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

#ifndef __FUNC_LIST_H
#define __FUNC_LIST_H

XMLDList *func_list;

short func_list_init();
short func_list_shutdown();
XMLDExpr *foo_func(XMLDList *);

#endif /* __FUNC_LIST_H */