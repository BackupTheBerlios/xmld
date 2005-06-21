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

#ifndef __XMLD_CFG_DIRECTIVE_H
#define __XMLD_CFG_DIRECTIVE_H

/* Represents a configuration directive */
struct XMLDCfgDirective {
 XMLDAssoc *values;
 char *name;
};

typedef struct XMLDCfgDirective XMLDCfgDirective;

XMLDCfgDirective *XMLDCfgDirective_create(void);
XMLDCfgValue *XMLDCfgDirective_get_value(XMLDCfgDirective *, char *, int);
void XMLDCfgDirective_free(XMLDCfgDirective *);

#endif /* __XMLD_CFG_DIRECTIVE_H */
