/*                                                                *
 * -------------------------------------------------------------- *
 * The OpenXMLD                                                   *
i * -------------------------------------------------------------- *
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
#define XMLD_INTEGER 0          /* Integer type              */
#define XMLD_OPERATION 1        /* Operation type            */
#define XMLD_IDENTIFIER 2       /* Identifier type           */
#define XMLD_FUNCTION 3         /* Function call type        */
#define XMLD_QVAL 4             /* Quoted value type         */ 
#define XMLD_WILDCARD 5         /* Wildcard type             */
#define XMLD_FLOAT 6            /* Floating point type       */
#define XMLD_LIST 7             /* Expression list type      */
#define XMLD_VOID_LIST 8        /* Void Expression list type */
#define XMLD_SPECIAL_IDENTIFIER /* Special Identifier type   */

/* XMLD-SQL Operation Types */
/* Arithmetic: */
#define XMLD_OP_ADD 0    /* Addition                  */
#define XMLD_OP_BNEG 1   /* Binary substraction       */
#define XMLD_OP_MULTIP 2 /* Multiplication            */
#define XMLD_OP_DIV 3    /* Division                  */
#define XMLD_OP_EXPO 4   /* Exponentation             */
#define XMLD_OP_UNEG 5   /* Sign switching            */

/* Checkers: */
#define XMLD_OP_EQUAL 6  /* Equals                    */
#define XMLD_OP_L 7      /* Less than                 */
#define XMLD_OP_G 8      /* Greater than              */
#define XMLD_OP_NE 9     /* Inequality                */
#define XMLD_OP_LE 10    /* Less than or equal        */
#define XMLD_OP_GE 11    /* Greater than or equal     */
#define XMLD_OP_LIKE 12  /* LIKE!		      */

/* Groupers: */
#define XMLD_OP_AND 13   /* AND grouping */
#define XMLD_OP_OR  14   /* OR grouping  */
#define XMLD_OP_NOT 15   /* Logical NOT  */


/* XMLD-SQL Special identifier types */
#define XMLD_SIDENT_TEXT 0    /* [text]    */
#define XMLD_SIDENT_TAGNAME 1 /* [tagname] */

/* XMLD-SQL Wildcard types */
#define XMLD_WILDCARD_ALL 0  /* All columns     */
#define XMLD_WILDCARD_ATTS 1 /* Attributes only */

struct XMLDExpr {
 int type; 
 XMLDBool aggr, cross_level;
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
 XMLDFile *file;
 /*--------------------*/
 int sident; /* type 9: a special identifier */
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
 int wildcard; /* type 5: wildcard */
 /*------------------*/
 float fnval; /* type 6: floating point number */
 /*------------------*/
 XMLDExprList *exprs; /* type 7: expression list */
 /*------------------*/
 char *alias;
};

#ifndef XMLDEXPR_TYPE_DEFINED
#define XMLDEXPR_TYPE_DEFINED
 typedef struct XMLDExpr XMLDExpr;
 typedef XMLDList XMLDExprList;
#endif /* XMLDEXPR_TYPE_DEFINED */

XMLDExpr *XMLDExpr_create(void);
void XMLDExpr_copy(XMLDExpr *, XMLDExpr *);
XMLDBool XMLDExpr_is_complex(XMLDExpr *);
void XMLDExpr_apply_type(XMLDExpr *, char *);
char *XMLDExpr_to_string(XMLDExpr *);
XMLDBool XMLDExpr_to_boolean(XMLDExpr *);
XMLDStatus XMLDExpr_to_columns(XMLDExpr *, XMLDWork *, int);
void XMLDExpr_free(XMLDExpr *);
void XMLDExpr_free_content(void *);

/* List functions */

XMLDExprList *XMLDExprList_create(void);
XMLDExpr *XMLDExprList_add(XMLDExprList *);

#endif /* __XMLD_EXPR_H */