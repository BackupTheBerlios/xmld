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

#ifndef HAVE_XMLD_DIRECTIVE_H
#define HAVE_XMLD_DIRECTIVE_H

union cfg_value {
  int int_value;
  int *int_array_value;
  char *string_value;
};

/* Represents a configuration directive */
struct XMLDDirective {
 char *name;
 union cfg_value value;
 short type; /* 0 = int
              * 1 = int*
	      * 2 = char*
	      */
};

typedef struct XMLDDirective XMLDDirective;

XMLDDirective *XMLDDirective_create(void);
void XMLDDirective_free_content(void *);
void XMLDDirective_free(XMLDDirective *);

/* List functions */

XMLDList *XMLDDirective_create_list(void);
XMLDDirective *XMLDDirective_add_to_list(XMLDList *);
XMLDDirective *XMLDDirective_search_list_by_name(XMLDList *, char *);

#endif /* HAVE_XMLD_DIRECTIVE_H */
