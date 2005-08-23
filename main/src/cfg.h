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
#define YY_BUF_SIZE 16384

CfgSection *cfg_tree;

char *document_root;

Status cfg_init(void);
void cfg_update(int);
Status cfg_shutdown(void);

#endif /* __CFG_H */
