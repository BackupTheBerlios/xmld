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

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

typedef void ProtocolResult;

struct Protocol {
 CfgSection *cfg;
 int modcap;

 ProtocolResult *(*init) (Assoc *);
 ProtocolResult *(*msg) (char *);
 ProtocolResult *(*terminate) (char *);
 
 Error *(*get_error) (ProtocolResult *);
 char *(*get_output) (ProtocolResult *);
 void (*destroy) (ProtocolResult *);
}

typedef struct Protocol Protocol;

#endif /* __PROTOCOL_H */
