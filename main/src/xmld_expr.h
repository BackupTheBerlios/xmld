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
 
#ifndef __XMLD_EXPR_H
#define __XMLD_EXPR_H

/* Expression types */
#define XMLD_INTEGER 0    /* Integer type         */
#define XMLD_OPERATION 1  /* Operation type       */
#define XMLD_IDENTIFIER 2 /* Identifier type      */
#define XMLD_FUNCTION 3   /* Function call type   */
#define XMLD_QVAL 4       /* Quoted value type    */ 
#define XMLD_WILDCARD 5   /* Wildcard type        */
#define XMLD_FLOAT 6      /* Floating point type  */

/* Operation types */
#define XMLD_OP_ADD 0    /* Addition                  */
#define XMLD_OP_BNEG 1   /* Binary substraction       */
#define XMLD_OP_MULTIP 2 /* Multiplication            */
#define XMLD_OP_DIV 3    /* Division                  */
#define XMLD_OP_EXPO 4   /* Exponentation             */
#define XMLD_OP_UNEG 5   /* Sign switching            */
#define XMLD_OP_AND 6    /* AND (for between support) */

/* Wildcard types */
#define XMLD_WILDCARD_ALL 0  /* All columns     */
#define XMLD_WILDCARD_ATTS 1 /* Only attributes */

struct XMLDExpr {
 int type; 
 XMLDBool aggr; /* Whether it's an aggregate expression */

 int nval; /* type 0 */
 /*--------------------*/
 struct XMLDExpr *left; /* type 1:
	                 * to represent an expression 
	                 * containing an operation op
	                 */
 struct XMLDExpr *right;
 int op; 
 /*-------------------*/
 char *ident; /* type 2 */
 /*-------------------*/
 XMLDFunc *func; /* type 3:
	          * a function call
		  * with an exprssion list
		  * being passed as an argument
		  */
 XMLDExprList *arg_list;
 /*-------------------*/
 char *qval; /* type 4: Quoted value */
 /*-------------------*/
 short wildcard; /* type 5: wildcard */
 /*------------------*/
 float fnval; /* type 6: floating point number */
 /*------------------*/
 char *alias;
};

typedef struct XMLDExpr XMLDExpr;
typedef XMLDList XMLDExprList;

XMLDExpr *XMLDExpr_create(void);
void XMLDExpr_copy(XMLDExpr *, XMLDExpr *);
void XMLDExpr_free(XMLDExpr *);
void XMLDExpr_free_content(void *);
/* List functions */

XMLDExprList *XMLDExprList_create(void);
XMLDExpr *XMLDExprList_add(XMLDExprList *);

#endif /* __XMLD_EXPR_H */
