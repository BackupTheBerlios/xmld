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
char *engine_xmld_get_attribute_type (XMLDFile *, char *);
char *engine_xmld_get_attribute (XMLDFile *, char *);
char *engine_xmld_get_text_type (XMLDFile *);
char *engine_xmld_get_text(XMLDFile *);
char *engine_xmld_get_tagname(XMLDFile *);

/* Sequential attribute selection API */

void engine_xmld_reset_element (XMLDFile *);
XMLDBool engine_xmld_next_attribute (XMLDFile *);
char *engine_xmld_get_curr_attribute_type (XMLDFile *);
char *engine_xmld_get_curr_attribute_name (XMLDFile *);
char *engine_xmld_get_curr_attribute_value (XMLDFile *);


#endif /* __ENGINE_XMLD_H */
