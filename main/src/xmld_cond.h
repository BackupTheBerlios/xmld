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

#ifndef __XMLD_COND_H
#define __XMLD_COND_H

/* Condition types */
#define XMLD_CONDITION 0      /* Normal condition: expr <cond> expr   */
#define XMLD_CONDITION_GRP 1  /* Condition group: cond <LOGICAL> cond */
#define XMLD_CONDITION_VOID 2 /* Void (condition|condition list)      */
#define XMLD_CONDITION_LIST 3 /* Condition list                       */

/* Conditional operation types */
#define XMLD_COND_OP_EQUAL 0 /* Equality              */
#define XMLD_COND_OP_L 1     /* Less than             */
#define XMLD_COND_OP_G 2     /* Greater than          */
#define XMLD_COND_OP_NE 3    /* Not Equal             */
#define XMLD_COND_OP_LE 4    /* Less than or equal    */
#define XMLD_COND_OP_GE 5    /* Greater than or equal */
#define XMLD_COND_OP_LIKE 6  /* LIKE                  */
#define XMLD_COND_OP_BET 7   /* BETWEEN               */
#define XMLD_COND_OP_NBET 8  /* NOT BETWEEN           */

/* Conditional grouping types */
#define XMLD_COND_GRP_AND 0 /* AND */
#define XMLD_COND_GRP_OR  1 /* OR  */

typedef XMLDList XMLDCondList;

/* represents a condition */
struct XMLDCond {
 int type;
 XMLDBool cross_level;
 /*---------*/
 XMLDExpr *left; /* type 0 */
 XMLDExpr *right;
 int op;  
 /*---------*/
 struct XMLDCond *cleft; /* type 1 */
 struct XMLDCond *cright;
 int cop;
 /*----------*/
 XMLDCondList *conds; /* type 3 */
 /*----------*/
 XMLDBool negate; /* Whether it's a negative condition */	
};

typedef struct XMLDCond XMLDCond;

XMLDCond *XMLDCond_create(void);
void XMLDCond_copy(XMLDCond *, XMLDCond *);
void XMLDCond_free(XMLDCond *);
void XMLDCond_free_content(void*);

/* List functions */

XMLDCondList *XMLDCondList_create(void);
XMLDCond *XMLDCondList_add(XMLDCondList *);

#endif /* __XMLD_COND_H */
