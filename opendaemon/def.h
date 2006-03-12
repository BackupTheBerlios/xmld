/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
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
#define OPENDAEMON_VERSION_MAJOR "0"
#define OPENDAEMON_VERSION_MINOR "8"
#define OPENDAEMON_VERSION_REVIEW "0"
#define OPENDAEMON_VERSION "0.8.0"

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
