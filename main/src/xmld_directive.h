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

#ifndef __XMLD_DIRECTIVE_H
#define __XMLD_DIRECTIVE_H

/* Directive types */
#define XMLD_DIR_ERR -1   /* Error         */
#define XMLD_DIR_INT 0    /* Integer       */
#define XMLD_DIR_INTARR 1 /* Integer array */
#define XMLD_DIR_STR 2    /* String        */
#define XMLD_DIR_STRARR 3 /* String array  */
#define XMLD_DIR_CHAR 4   /* Character     */

union cfg_value {
  int int_value;
  int *int_array_value;
  char *string_value;
  char **string_array_value;
  char char_value;
};

/* Represents a configuration directive */
struct XMLDDirective {
 char *name;
 union cfg_value value;
 int type;
};

typedef struct XMLDDirective XMLDDirective;

XMLDDirective *XMLDDirective_create(void);
void XMLDDirective_free_content(void *);
void XMLDDirective_free(XMLDDirective *);

/* List functions */

XMLDList *XMLDDirective_create_list(void);
XMLDDirective *XMLDDirective_add_to_list(XMLDList *);
XMLDDirective *XMLDDirective_search_list_by_name(XMLDList *, char *);

#endif /* __XMLD_DIRECTIVE_H */
