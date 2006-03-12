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

#ifndef __CFG_SECTION_H
#define __CFG_SECTION_H

struct CfgSection {
 Assoc *directives;
 Assoc *sections;
 char *name;
};

#ifndef CFG_SECTION_TYPE_DEFINED
#define CFG_SECTION_TYPE_DEFINED
 typedef struct CfgSection CfgSection;
#endif /* CFG_SECTION_TYPE_DEFINED */

CfgSection *CfgSection_create(void);
CfgSection *CfgSection_get_section(CfgSection *, char *, int);
CfgDirective *CfgSection_get_directive(CfgSection *, char *, int);
void CfgSection_free(CfgSection *);

#endif /* __CFG_SECTION_H */
