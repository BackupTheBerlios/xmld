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

#ifndef HAVE_XMLD_TYPES_H
#define HAVE_XMLD_TYPES_H

/* represents an expression */
struct expr {
 short type; /* 0 = numeric 
	    * 1 = containing an operation
	    * 2 = IDENTIFIER (column name)
	    * 3 = function call
	    * 4 = quoted value
	    * 5 = wildcard
	    */
 
 short aggr; /* whether or not it's an aggregate expression
	      * aggregate if type = 5 (wildcard) or if type
	      * = 3 (function call) with this function being
	      * an aggregate one.
	      * ------
	      * NOTE:
	      *   1 = aggregate
	      *   0 = scalar
	      */
 
 int nval; /* type 0 */
 
 /*--------------------*/
 struct expr *left; /* type 1:
		     * to represent an expression 
		     * containing an operation op
		     */
 struct expr *right;
 short op; /* 0 = '+'
	    * 1 = '-' (binary negative)
	    * 2 = '*'
	    * 3 = '/'
	    * 4 = '^'
	    * 5 = '-' (unary negative)
	    */ 
 /*-------------------*/
 char *ident; /* type 2 */
 /*-------------------*/
 /* FIXME: replace this with an XMLDFunc struct
  * that carries whether the function is aggregate 
  */
 void *(*func) (struct expr *); /* type 3:
				 * a function call
				 * with an exprssion array
				 * being passed as an argument
				 */
 struct expr *arg_list;
 /*-------------------*/
 char *qval; /* type 4: Quoted value */
 /*-------------------*/
 short wildcard; /* type 5: wildcard 
		  * 0 = '*'
		  * 1 = '@'
		  */
 /*------------------*/
 char *alias;
};

/* represents a condition */
struct cond {
 short type; /*
	      * 0 = Condition (expr <condition> expr)
	      * 1 = Condition Group (cond <operation> cond)
	      */

 /*---------*/
 struct expr *left; /* type 0 */
 struct expr *right;
 short op; /*
	    * 0 = '='
	    * 1 = '<'
	    * 2 = '>'
	    * 3 = '<>'
	    * 4 = '<='
	    * 5 = '>='
	    * 6 = LIKE
	    * 7 = BETWEEN
	    * 8 = NOT BETWEEN
	    */ 
 /*---------*/
 struct cond *cleft; /* type 1 */
 struct cond *cright;
 short cop;  /*
	      * 0 = AND
	      * 1 = OR
	      */
 /*----------*/
 short negate; /* 0 = Affirmative condition
	        * 1 = Negative condition
	        */	
};

/* carries the parsed query */
struct XMLDRequest {
 char *file;
 short type; /* type of the query
	      * 0 = SELECT query
	      * 1 = SELECT with WHERE
	      */
 struct expr **retr;  /* the list of expressions to be
		      * retrieved 
		      */
 struct cond **where; /* A condition for each level */
};

/* carries the response to be sent */
struct XMLDResponse {
};

/* carries an engine's information */
struct XMLDEngine {
 void *(*init) (struct XMLDWork *);
 void *(*destroy) (struct XMLDWork *);
};

/* carries per-request resources */
struct XMLDResource {
 struct XMLDEngine *engine; /* the engine to be used */
 void *data_source; /* the data_source (fd/memory tree) */
};

/* carries connection-specific information */
struct XMLDConnection {
 int fd;
 int sfd;
 char *curr_dir; /* the current directory */
};

struct XMLDWork {
 struct XMLDRequest *req;
 struct XMLDResponse *resp;
 struct XMLDResource *res;
 struct XMLDConnection *conn;
};

void expr_free_content(void *);
#endif
