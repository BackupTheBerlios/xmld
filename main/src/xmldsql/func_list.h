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

XMLDFuncList *func_list;

XMLDStatus func_list_init();
XMLDStatus func_list_shutdown();
XMLDExpr *foo_func(XMLDExprList *, XMLDFile *);

#endif /* __FUNC_LIST_H */
