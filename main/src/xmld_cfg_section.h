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

#ifndef __XMLD_CFG_SECTION_H
#define __XMLD_CFG_SECTION_H

struct XMLDCfgSection {
 char *name;
 XMLDCfgDirectiveList *directives;
 XMLDCfgSectionList *sections;
};

#ifndef XMLD_CFG_SECTION_TYPE_DEFINED
#define XMLD_CFG_SECTION_TYPE_DEFINED
 typedef struct XMLDCfgSection XMLDCfgSection;
 typedef XMLDList XMLDCfgSectionList;
#endif /* XMLD_CFG_SECTION_TYPE_DEFINED */

XMLDCfgSection *XMLDCfgSection_create(void);
void XMLDCfgSection_free_content(void *);
void XMLDCfgSection_free(XMLDCfgSection *);

/* List functions */

XMLDCfgSectionList *XMLDCfgSectionList_create(void);
XMLDCfgSection *XMLDCfgSectionList_add(XMLDCfgSectionList *);
XMLDCfgSection *XMLDCfgSectionList_search_by_name(XMLDCfgSectionList *, char *);

#endif /* __XMLD_CFG_SECTION_H */
