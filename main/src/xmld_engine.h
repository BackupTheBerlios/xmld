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
 
#ifndef HAVE_XMLD_ENGINE_H
#define HAVE_XMLD_ENGINE_H

struct XMLDEngine {
 char *name; /* The engine's name */
 void (*init) (void); /* The function that's called once the engine
		       * is added to the engine_list.
		       */
 short (*prepare) (XMLDWork *); /* The function called before each
				 * request the engine serves.
				 */
 
 void (*cleanup) (XMLDWork *); /* The function called after each
				*  request the engine serves.
				*/
 void (*destroy) (void); /* The function called before the engine
			  * is removed from engine_list.
			  */
 
 int (*walk) (XMLDWork *); /* The function which the tree walker
			    * calls for the engine to give a next
			    * result from its data source. 
			    */
 
 char *(*eval_expr) (XMLDWork *, XMLDExpr *); /* The function which
					       * the tree walker calls
					       * to get the value of
					       * an expression from the
					       * engine. 	 
					       */
 
 short (*eval_cond) (XMLDWork *, XMLDCond *); /* The function which
					       * the tree walker calls
					       * when it needs the eng-
					       * ine to evaluate a con-
					       * dition.
					       */
 
 char *(*eval_aggr_expr) (XMLDWork *, XMLDExpr *); /* The function which
						    * the tree walker calls
						    * when it needs the en-
						    * gine to evaluate an 
						    * aggregate expression.                                                   *
						    */  
};

#ifndef XMLDENGINE_TYPE_DEFINED
#define XMLDENGINE_TYPE_DEFINED
 typedef struct XMLDEngine XMLDEngine;
#endif /* XMLDENGINE_TYPE_DEFINED */

XMLDEngine *XMLDEngine_create(char *);
void XMLDEngine_free(XMLDEngine *);
void XMLDEngine_free_content(void *);

/* List functions */

XMLDList *XMLDEngine_create_list(void);
XMLDEngine *XMLDEngine_add_to_list(XMLDList *, char *);
XMLDEngine *XMLDEngine_search_list_by_name(XMLDList *, char *);

#endif /* HAVE_XMLD_ENGINE_H */
