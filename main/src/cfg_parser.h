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

#ifndef __CFG_PARSER_H
#define __CFG_PARSER_H

/* Parser modes (looking for:) */
#define XMLD_CFG_MODE_TYPE 0  /* Type        */
#define XMLD_CFG_MODE_DIR 1   /* Directive   */
#define XMLD_CFG_MODE_EQ 2    /* Equal sign  */
#define XMLD_CFG_MODE_VAL 3   /* Value       */
#define XMLD_CFG_MODE_END 4   /* End of line */

XMLDList *cfg_tree;

XMLDStatus cfg_parser_parse(void);
void cfg_parser_parse_token(char *, int);
XMLDStatus cfg_parser_clean(void);

#endif /* __CFG_PARSER_H */
