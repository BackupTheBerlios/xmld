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
 
#ifndef __ELEMENT_OP_H
#define __ELEMENT_OP_H

char *engine_xmld_get_curr_att_name (FILE *);
char *engine_xmld_get_curr_att_value (FILE *);
void engine_xmld_discard_curr_att_value(FILE *);
char *engine_xmld_get_text_value(FILE *);
char *engine_xmld_get_tagname(FILE *);
void engine_xmld_discard_curr_att_name(FILE *);
short engine_xmld_locate_text(FILE *);
short engine_xmld_locate_att(FILE *, char *);
short engine_xmld_set_text_value(FILE *, char *);
short engine_xmld_set_curr_att_value(FILE *, char *);

#endif /* __ELEMENT_OP_H */
