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
 
#ifndef __QP_H
#define __QP_H

#define QP_COL_SEP_FIELD "col-sep"
#define QP_COL_SEP_ENC_FIELD "col-sep-enc"
#define QP_ROW_SEP_FIELD "row-sep"
#define QP_ROW_SEP_ENC_FIELD "row-sep-enc"
#define QP_DOWN_LEVEL_FIELD "down-level"
#define QP_DOWN_LEVEL_ENC_FIELD "down-level-enc"
#define QP_UP_LEVEL_FIELD "up-level"
#define QP_UP_LEVEL_ENC_FIELD "up-level-enc"

void qp_handle(void *conn);
int yyparse(void *);
int yywrap(void);
void yyerror(char *);

#endif /* __QP_H */
