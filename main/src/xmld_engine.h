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
 
#ifndef __XMLD_ENGINE_H
#define __XMLD_ENGINE_H

/* Access level bits */
#define XMLD_ACCESS_NOTHING 0    /* Nothing special                                */
#define XMLD_ACCESS_FORMAT 1     /* Whether to open a format file                  */
#define XMLD_ACCESS_EX 2         /* Whether to execlusively access the file        */
#define XMLD_ACCESS_FORMAT_EX 4  /* Whether to execlusively access the format file */

/* walk return values */
#define XMLD_WALK_UP -1  /* One level shallower */
#define XMLD_WALK_END 0  /* End of document     */
#define XMLD_WALK_DOWN 1 /* One level deeper    */

struct XMLDEngine {
 char *name; /* The engine's name */
 void (*init) (void); /* The function that's called once the engine
		       * is added to the engine_list.
		       */
 XMLDStatus (*prepare) (XMLDWork *, XMLDFile *, int); /* The function called before each
				                       * request the engine serves.
				                       * The integer argument is named level
				                       * and determines access levels of the
				                       * request. (see the access level bits
				                       * above)
				                       */
 
 void (*cleanup) (XMLDWork *, XMLDFile *); /* The function called after each
				            * request the engine serves.
				            */
 void (*destroy) (void); /* The function used for removal of the 
			  * engine from engine_list.
			  */
 
 int (*walk) (XMLDWork *, XMLDFile *); /* The function which makes the engine
			                * give a next result from the given file.
			                */
 char *(*eval_expr) (XMLDWork *, XMLDExpr *, int); /* The function used for
					            * getting the value of
					            * an expression. 	 
					            */
 
 XMLDBool (*eval_cond) (XMLDWork *, XMLDCond *, int); /* The function used for
					               * evaluation of a con-
					               * dition.
					               */
};

#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */
typedef XMLDList XMLDEngineList;

XMLDEngine *XMLDEngine_create(char *);
void XMLDEngine_free(XMLDEngine *);
void XMLDEngine_free_content(void *);

/* List functions */

XMLDEngineList *XMLDEngineList_create(void);
XMLDEngine *XMLDEngineList_add(XMLDEngineList *, char *);
XMLDEngine *XMLDEngineList_search_by_name(XMLDEngineList *, char *);

#endif /* __XMLD_ENGINE_H */
