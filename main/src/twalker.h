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

#ifndef __TWALKER_H
#define __TWALKER_H

XMLDStatus twalker_handle(XMLDWork *);
XMLDExpr *twalker_simplify_expr(XMLDExpr *, XMLDWork *, int);

#endif /* __TWALKER_H */
