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
 
#ifndef HAVE_ENGINE_XMLD_H
#define HAVE_ENGINE_XMLD_H

void engine_xmld_init (void);
short engine_xmld_prepare (XMLDWork *);
void engine_xmld_cleanup (XMLDWork *);
void engine_xmld_destroy (void);
short engine_xmld_walk (XMLDWork *);
char *engine_xmld_eval_expr (XMLDWork *, XMLDExpr *);
short engine_xmld_eval_cond (XMLDWork *, XMLDCond *);
char *engine_xmld_eval_aggr_expr (XMLDWork *, XMLDExpr *);

#endif /* HAVE_ENGINE_XMLD_H */
