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

#ifndef __DEF_H
#define __DEF_H

/* Version defines */
#define VERSION_MAJOR "0"
#define VERSION_MINOR "7"
#define VERSION_REVIEW "0"
#define VERSION "0.7.0"

/* Status values */
#define SUCCESS 1
#define FAILURE 0

/* Bool values */
#define FALSE 0
#define TRUE 1

#define BIT_ISSET(bitmask, bit) ((bitmask) & (bit))

typedef int Bool;
typedef int Status;

#endif /* __DEF_H */
