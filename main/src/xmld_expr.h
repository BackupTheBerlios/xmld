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
 
#ifndef HAVE_XMLD_EXPR_H
#define HAVE_XMLD_EXPR_H

struct XMLDExpr {
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
 struct XMLDExpr *left; /* type 1:
		                     * to represent an expression 
		                     * containing an operation op
		                     */
 struct XMLDExpr *right;
 short op; /* 0 = '+'
	    * 1 = '-' (binary negative)
	    * 2 = '*'
	    * 3 = '/'
	    * 4 = '^'
	    * 5 = '-' (unary negative)
	    * 6 = AND (for between and not between)
	    */ 
 /*-------------------*/
 char *ident; /* type 2 */
 /*-------------------*/
 XMLDFunc *func; /* type 3:
				      * a function call
				      * with an exprssion array
				      * being passed as an argument
				      */
 XMLDList *arg_list;
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

typedef struct XMLDExpr XMLDExpr;

XMLDExpr *XMLDExpr_create(void);
void XMLDExpr_free(XMLDExpr *);
void XMLDExpr_free_content(void *);
/* List functions */

XMLDList *XMLDExpr_create_list(void);
XMLDExpr *XMLDExpr_add_to_list(XMLDList *);

#endif /* HAVE_XMLD_EXPR_H */
