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

#ifndef __XMLD_PARSER_H
#define __XMLD_PARSER_H

/* Represents a query parser */
struct XMLDParser {
 char *name; /* Parser name */
 void (*init) (void);
 void (*destroy) (void);
 void (*prepare) (XMLDWork *);
 void (*parse) (XMLDWork *, char *query);
 void (*walk) (XMLDWork *);
 char *(*get_response) (XMLDWork *);
 void (*cleanup) (XMLDWork *);
};

typedef struct XMLDParser XMLDParser;
typedef struct XMLDList XMLDParserList;

XMLDParser *XMLDParser_create(void);
void XMLDParser_free(XMLDParser *);
void XMLDParser_free_content(void *);

/* List functions */

XMLDParserList *XMLDParserList_create(void);
XMLDParser *XMLDParserList_add(XMLDParserList *);

#endif /* __XMLD_PARSER_H */
