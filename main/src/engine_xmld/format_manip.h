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

#ifndef __FORMAT_MANIP_H
#define __FORMAT_MANIP_H

FILE *engine_xmld_load_format_file(char *, short);
int engine_xmld_get_element_att_length(FILE *, int, char *, char *);
char *engine_xmld_get_element_att_type(FILE *, int, char *, char *);

#endif /* __FORMAT_MANIP_H */
