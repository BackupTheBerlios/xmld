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

#ifndef __XMLD_SQL_H
#define __XMLD_SQL_H

/* XMLD-SQL Request types */
#define XMLD_SQL_SELECT 0           /* SELECT                             */
#define XMLD_SQL_SELECT_WHERE 1     /* SELECT with WHERE                  */
#define XMLD_SQL_UPDATE 2           /* UPDATE                             */
#define XMLD_SQL_UPDATE_WHERE 3     /* UPDATE with WHERE                  */
#define XMLD_SQL_DELETE 4           /* DELETE                             */
#define XMLD_SQL_DELETE_WHERE 5     /* DELETE with WHERE                  */
#define XMLD_SQL_INSERT_COL 6       /* INSERT with column list            */
#define XMLD_SQL_INSERT_COL_WHERE 7 /* INSERT with column list with WHERE */
#define XMLD_SQL_INSERT_WHERE 8     /* INSERT with WHERE                  */
#define XMLD_SQL_INSERT 9           /* INSERT                             */
#define XMLD_SQL_USE 10             /* USE                                */

#endif /* __XMLD_SQL_H */

