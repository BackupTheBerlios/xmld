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
 
#ifndef __ENGINE_XMLD_H
#define __ENGINE_XMLD_H

void engine_xmld_init (void);
XMLDStatus engine_xmld_prepare (XMLDWork *, XMLDFile *, int);
void engine_xmld_cleanup (XMLDWork *, XMLDFile *);
void engine_xmld_destroy (void);
int engine_xmld_walk (XMLDWork *, XMLDFile *);
char *engine_xmld_eval_expr (XMLDWork *, XMLDExpr *, XMLDAggrTable *, int);
XMLDBool engine_xmld_eval_cond (XMLDWork *, XMLDCond * int);
char *engine_xmld_get_column_value (XMLDWork *, char *);
XMLDStatus engine_xmld_set_column_value (XMLDWork *, char *, char *);
void engine_xmld_simplify_expr (XMLDWork *, XMLDExpr *);
