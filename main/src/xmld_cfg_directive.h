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
 char *name;
 XMLDCfgValueList *values;
};

typedef struct XMLDCfgDirective XMLDCfgDirective;
typedef XMLDList XMLDCfgDirectiveList;

XMLDCfgDirective *XMLDCfgDirective_create(void);
void XMLDCfgDirective_free_content(void *);
void XMLDCfgDirective_free(XMLDCfgDirective *);

/* List functions */

XMLDCfgDirectiveList *XMLDCfgDirectiveList_create(void);
XMLDCfgDirective *XMLDCfgDirectiveList_add(XMLDCfgDirectiveList *);
XMLDCfgDirective *XMLDCfgDirectiveList_search_by_name(XMLDCfgDirectiveList *, char *);

#endif /* __XMLD_CFG_DIRECTIVE_H */
