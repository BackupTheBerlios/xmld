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
 
#ifndef __ENGINE_LIST_H
#define __ENGINE_LIST_H

XMLDEngineList *engine_list;

XMLDStatus engine_list_init();
char *engine_list_get_engine(char *);
void engine_list_set_engine(char *, char *);
char *engine_list_get_mime_engine(char *);
void engine_list_set_mime_engine(char *, char *);
XMLDStatus engine_list_shutdown();

#endif /* __ENGINE_LIST_H */
