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

#ifndef HAVE_XMLD_COND_H
#define HAVE_XMLD_COND_H

/* represents a condition */
struct XMLDCond {
 short type;
          /* 0 = Condition (expr <condition> expr)
	         * 1 = Condition Group (cond <operation> cond)
	         */

 /*---------*/
 XMLDExpr *left; /* type 0 */
 XMLDExpr *right;
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
 struct XMLDCond *cleft; /* type 1 */
 struct XMLDCond *cright;
 short cop;  /*
	      * 0 = AND
	      * 1 = OR
	      */
 /*----------*/
 short negate; /* 0 = Affirmative condition
	              * 1 = Negative condition
	              */	
};

typedef struct XMLDCond XMLDCond;

void XMLDCond_free_content(XMLDCond *);
void XMLDCond_free(XMLDCond *);

#endif /* HAVE_XMLD_COND_H */