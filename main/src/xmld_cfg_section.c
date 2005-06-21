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

#include "includes.h"
#include "xmld_cfg_directive.h"
#include "xmld_cfg_section.h"

/*
 * : Creates a new configuration section structure.
 * returns: the newly created section.
 */
XMLDCfgSection *XMLDCfgSection_create() {
 XMLDCfgSection *section = (XMLDCfgSection *) malloc(sizeof(XMLDCfgSection));
 section->directives = NULL;
 section->sections = NULL;
 section->name = NULL;
 return section;
}


/*
 * : Gets a child section of the given section
 * that has the given name, if multiple sections
 * with that name exist, the given index is to
 * decide which one to return.
 */
XMLDCfgSection *XMLDCfgSection_get_section(XMLDCfgSection *section, char *name, int index) {
 return XMLDAssoc_get_key_index(section->sections, name, index);
}

/*
 * : Gets a directive in the given section
 * that has the given name, if multiple directives
 * with that name exist, the given index is to
 * decide which one to return.
 */
XMLDCfgDirective *XMLDCfgSection_get_directive(XMLDCfgSection *section, char *name, int index) {
 return XMLDAssoc_get_key_index(section->directives, name, index);
}

/*
 * : Frees a configuration section structure.
 * section: the section structure to free.
 */
void XMLDCfgSection_free(XMLDCfgSection *section) {
 if (section != NULL) {
  XMLDAssocWalker walker;
  walker.subject = section->directives;
  walker.curr_index = -1;
  while (XMLDAssocWalker_next(&walker)) {
   if (XMLDAssocWalker_get_current_data(&walker) != NULL) {
    XMLDCfgDirective_free((XMLDCfgDirective *) XMLDAssocWalker_get_current_data(&walker));
   }
  }
  XMLDAssoc_free(section->directives);
  walker.subject = section->sections;
  walker.curr_index = -1;
  while (XMLDAssocWalker_next(&walker)) {
   if (XMLDAssocWalker_get_current_data(&walker) != NULL) {
    XMLDCfgSection_free((XMLDCfgSection *) XMLDAssocWalker_get_current_data(&walker));
   }
  }
  XMLDAssoc_free(section->sections);
  free(section);
 } 
}
