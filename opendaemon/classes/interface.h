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

struct Interface {
 CfgSection *cfg;
 InterfaceData *data;
 
 Status (*init) (Interface *);
 void (*main) (Interface *);
 void (*destroy) (Interface *);

 Error *(*get_error) (Interface *);
};

typedef struct Interface interface;

#endif /* __INTERFACE_H */
