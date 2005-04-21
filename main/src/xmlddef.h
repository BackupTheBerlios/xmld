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

#ifndef __XMLDDEF_H
#define __XMLDDEF_H

/* Version defines */
#define XMLD_VERSION_MAJOR "0"
#define XMLD_VERSION_MINOR "7"
#define XMLD_VERSION_REVIEW "0"
#define XMLD_VERSION "0.7.0"

/* XMLDStatus values */
#define XMLD_SUCCESS 1
#define XMLD_FAILURE 0

/* XMLDBool values */
#define XMLD_FALSE 0
#define XMLD_TRUE 1

#define BIT_ISSET(bitmask, bit) ((bitmask) & (bit))

typedef int XMLDBool;
typedef int XMLDStatus;

#endif /* __XMLDDEF_H */
