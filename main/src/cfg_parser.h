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

XMLDCfgSection *cfg_tree;

XMLDStatus cfg_parser_parse(void);
XMLDStatus cfg_parser_clean(void);

#endif /* __CFG_PARSER_H */
