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

#ifndef HAVE_CFG_PARSER_H
#define HAVE_CFG_PARSER_H

XMLDList *cfg_tree;

short cfg_parser_parse(void);
void cfg_parser_parse_token(char *, short);
short cfg_parser_clean(void);

#endif /* HAVE_CFG_PARSER_H */