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

#ifndef __XMLD_CFG_VALUE_H
#define __XMLD_CFG_VALUE_H

/* Directive types */
#define XMLD_CFG_INTEGER 0    /* Integer */
#define XMLD_CFG_STRING  1    /* String  */

/* Represents a configuration directive */
struct XMLDCfgValue {
 int type;
 void *value;
};

typedef struct XMLDCfgValue XMLDCfgValue;

XMLDCfgValue *XMLDCfgValue_create(void);
void XMLDCfgValue_free_content(void *);
void XMLDCfgValue_free(XMLDCfgValue *);

#endif /* __XMLD_CFG_VALUE_H */
