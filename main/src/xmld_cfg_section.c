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
 section->name=NULL;
 section->directives = NULL;
 section->sections = NULL;
 return section;
}


/*
 * : Gets a child section of the given section
 * that has the given name, if multiple sections
 * with that name exist, the given index is to
 * decide which one to return.
 */
XMLDCfgSection *XMLDCfgSection_get_section(XMLDCfgSection *section, char *name, int index) {
 int num = 0;
 XMLDList_reset(section->sections);
 while (XMLDList_next(section->sections)) {
  if (strcmp(((XMLDCfgSection *) XMLDList_curr(section->sections))->name, name) == 0) {
   num++;
   if (num == index) {
    return (XMLDCfgSection *) XMLDList_curr(section->sections);
    break;
   }
  }
 }
 return NULL;
}

/*
 * : Gets a directive in the given section
 * that has the given name, if multiple directives
 * with that name exist, the given index is to
 * decide which one to return.
 */
XMLDCfgDirective *XMLDCfgSection_get_directive(XMLDCfgSection *section, char *name, int index) {
 int num = 0;
 XMLDList_reset(section->directives);
 while (XMLDList_next(section->directives)) {
  if (strcmp(((XMLDCfgDirective *) XMLDList_curr(section->directives))->name, name) == 0) {
   num++;
   if (num == index) {
    return (XMLDCfgDirective *) XMLDList_curr(section->directives);
   }
  }
 }
 return NULL;
}

/*
 * : Frees a configuration section structure.
 * section: the section structure to free.
 */
void XMLDCfgSection_free(XMLDCfgSection *section) {
 if (section != NULL) {
  XMLDCfgSection_free_content((void *) section);
  free(section);
 } 
};

/*
 * : Frees the memory internally allocated
 * by a configuration section structure.
 * section: the configuration section structure
 * whose internal memory is to be freed.
 */
void XMLDCfgSection_free_content(void *section) {
 XMLDCfgSection *section = (XMLDCfgSection *) section;
 XMLDList_free(section->directives);
 XMLDList_free(section->sections);
 if (section->name != NULL) {
  free(section->name);
 }
}
