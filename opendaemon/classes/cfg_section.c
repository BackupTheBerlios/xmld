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

#include "../includes.h"

/*
 * : Creates a new configuration section structure.
 * returns: the newly created section.
 */
CfgSection *CfgSection_create() {
 CfgSection *section = (CfgSection *) malloc(sizeof(CfgSection));
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
CfgSection *CfgSection_get_section(CfgSection *section, char *name, int index) {
 return Assoc_get_key_index(section->sections, (void *) name, index);
}

/*
 * : Gets a directive in the given section
 * that has the given name, if multiple directives
 * with that name exist, the given index is to
 * decide which one to return.
 */
CfgDirective *CfgSection_get_directive(CfgSection *section, char *name, int index) {
 return Assoc_get_key_index(section->directives, name, index);
}

/*
 * : Frees a configuration section structure.
 * section: the section structure to free.
 */
void CfgSection_free(CfgSection *section) {
 if (section != NULL) {
  AssocWalker walker;
  walker.subject = section->directives;
  walker.curr_index = -1;
  while (AssocWalker_next(&walker)) {
   if (AssocWalker_get_current_data(&walker) != NULL) {
    CfgDirective_free((CfgDirective *) AssocWalker_get_current_data(&walker));
   }
  }
  Assoc_free(section->directives);
  walker.subject = section->sections;
  walker.curr_index = -1;
  while (AssocWalker_next(&walker)) {
   if (AssocWalker_get_current_data(&walker) != NULL) {
    CfgDirective_free((CfgDirective *) AssocWalker_get_current_data(&walker));
   }
  }
  Assoc_free(section->sections);
  free(section);
 } 
}
