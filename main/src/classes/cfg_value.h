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

#ifndef __CFG_VALUE_H
#define __CFG_VALUE_H

/* Directive types */
#define CFG_INTEGER 0    /* Integer */
#define CFG_STRING  1    /* String  */

/* Represents a configuration directive */
struct CfgValue {
 int type;
 void *value;
};

typedef struct CfgValue CfgValue;

CfgValue *CfgValue_create(void);
void CfgValue_free(CfgValue *);

#endif /* __CFG_VALUE_H */
