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
 
#ifndef __INTERFACE_H
#define __INTERFACE_H

typedef void InterfaceData;

struct Interface {
 CfgSection *cfg;
 InterfaceData *data;
 
 Status (*init) (struct Interface *);
 void (*main) (struct Interface *);
 void (*destroy) (struct Interface *);

 Error *(*get_error) (struct Interface *);
};

typedef struct Interface Interface;

#endif /* __INTERFACE_H */
