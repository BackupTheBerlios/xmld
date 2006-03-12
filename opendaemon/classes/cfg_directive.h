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

#ifndef __CFG_DIRECTIVE_H
#define __CFG_DIRECTIVE_H

/* Represents a configuration directive */
struct CfgDirective {
 Assoc *values;
 char *name;
};

typedef struct CfgDirective CfgDirective;

CfgDirective *CfgDirective_create(void);
CfgValue *CfgDirective_get_value(CfgDirective *, int);
void CfgDirective_free(CfgDirective *);

#endif /* __CFG_DIRECTIVE_H */
