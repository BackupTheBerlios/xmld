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

#ifndef __CFG_H
#define __CFG_H

XMLDCfgSection *cfg_tree;

char *document_root;

XMLDStatus cfg_init(void);
void *cfg_get(char *, char *, XMLDCfgSection *);
XMLDStatus cfg_shutdown(void);

#endif /* __CFG_H */
