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

#ifndef __PROCESSOR_H
#define __PROCESSOR_H

typedef void ProcessResult;

struct Processor {
 CfgSection *cfg;
 int modcap;

 ProcessResult *(*process) (char *, Assoc *);
 
 void *(*get_output) (ProcessResult *);
 void (*destroy) (ProcessResult *);
 
 Error *(*get_error) (ProcessResult *);
}

typedef struct Processor Processor;

#endif /* __PROCESSOR_H */
