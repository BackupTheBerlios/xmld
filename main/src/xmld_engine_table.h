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
 
#ifndef HAVE_XMLD_ENGINE_TABLE_H
#define HAVE_XMLD_ENGINE_TABLE_H

struct XMLDEngineTable {
 XMLDEngine *content;
 XMLDEngine *last_element;
 XMLDEngine *curr_element; 
};

typedef struct XMLDEngineTable XMLDEngineTable;

XMLDEngineTable *XMLDEngineTable_create(void);
void XMLDEngineTable_free_content(XMLDEngineTable *);
void XMLDEngineTable_free(XMLDEngineTable *);
void XMLDEngineTable_add(XMLDEngineTable *, void *(*)(XMLDWork *), void *(*)(XMLDWork *), int (*)(XMLDWork *), char *(*)(XMLDWork *, XMLDExpr *), int (*) (XMLDWork *, XMLDCond *), char *(*) (XMLDWork *, XMLDExpr *));
void XMLDEngineTable_reset(XMLDEngineTable *);
void XMLDEngineTable_next(XMLDEngineTable *);
void XMLDEngineTable_prev(XMLDEngineTable *);
XMLDEngine *XMLDEngineTable_curr(XMLDEngineTable *);

#endif /* HAVE_XMLD_ENGINE_TABLE_H */