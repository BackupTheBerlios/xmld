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
 
#include "../includes.h"
#include "../cfg.h"
#include "../mfigure.h"
#include "../fmanager.h"
#include "engine_xmld.h"
#include "format_manip.h"

/* init function */
void engine_xmld_init() {
 /* 
  * init is useless in engine_xmld.
  * but it might be useful for persistent things which may appear later (?)
  */
}

/* prepare function 
 * level: see xmld_engine.h
 * */
XMLDStatus engine_xmld_prepare(XMLDWork *work, XMLDFile *file, int level) {
 char *full_name=XMLDFile_get_full_name(file, work);
 char *mime=mfigure_get_mime(full_name);
 
 if (strcmp(mime, "text/xml") != 0) { /* A XML-only engine! */
  xmld_errno=XMLD_EINVALFILE;
  free(mime);
  free(full_name);
  return XMLD_FAILURE;
 }
 
 free(mime);
 if (BIT_ISSET(level, XMLD_ACCESS_EX)) {
  file->data=(void *) fmanager_get_ex_fd(full_name);
 }
 else {
  file->data=(void *) fmanager_get_sh_fd(full_name);
 }
 
 if (file->data == NULL) {
  xmld_errno = XMLD_ENOFILE;
  free(full_name);
  return XMLD_FAILURE;
 }
 
 file->level=0;
 if (BIT_ISSET(level, XMLD_ACCESS_FORMAT)) {	 
  file->store=(void *) engine_xmld_load_format_file(full_name, BIT_ISSET(level, XMLD_ACCESS_FORMAT_EX));
  if (file->store == NULL) {
   xmld_errno = XMLD_ENOFORMAT;
   return XMLD_FAILURE;
  }  
 }
 else {
  file->store = NULL;
 }
 free(full_name);
 return XMLD_SUCCESS;
}
/* cleanup function */
void engine_xmld_cleanup(XMLDWork *work, XMLDFile *file) {
 if (file->store != NULL) {
  fmanager_unlock_fd((FILE *) file->store);
  fclose((FILE *) file->store);
 }
 fmanager_unlock_fd((FILE *) file->data);
 fclose((FILE *) file->data);
}

/* destroy function */
void engine_xmld_destroy() {
}

/* walk function
 * return values:
 * XMLD_WALK_UP  : shallower by one
 * XMLD_WALK_END : end of document
 * XMLD_WALK_DOWN: deeper by one
 */ 
int engine_xmld_walk(XMLDWork *work, XMLDFile *file) {
 short token;
 char buf;
 char *tokens[2]={"<", "/>"};
  
 while (1) {
  token=dmstrstr((FILE *) file->data, tokens, 2);
  if (token == -1) {
   return XMLD_WALK_END;
  }
  else if (token == 0) {
   buf=getc((FILE *) file->data);
   if (buf == '/') {
    file->level--;
    return XMLD_WALK_UP;
   }
   else {
    fseek((FILE *) file->data, -1, SEEK_CUR);
    file->level++;
    fgetpos((FILE *) file->data, &file->element_pos);
    return XMLD_WALK_DOWN;
   } 
  }
  else if (token == 1) {
   file->level--;
   return XMLD_WALK_UP;
  }
 }
}

/* Sequential attribute selection API */

void engine_xmld_reset_element(XMLDFile *file) {
 fsetpos((FILE *) file->data, &file->element_pos);
}

XMLDBool *engine_xmld_next_attribute(XMLDFile *file) {
 int tok = dmstrchr((FILE *) file->data, " >", 2);
 if (tok == 1) {
  engine_xmld_reset_element(file);
 }
 if (tok == 0) {
  return XMLD_TRUE;
 }
 else {
  return XMLD_FALSE;
 }
}

char *engine_xmld_get_curr_attribute_type(XMLDFile *file) {
 if (file->store == NULL) {
  return NULL;
 }
 char *att = engine_xmld_get_curr_attribute_name(XMLDFile *file);
 return engine_xmld_get_attribute_type(file, att);
}

char *engine_xmld_get_curr_attribute_name(XMLDFile *file) {
 return dmcstrchr((FILE *) file->data, "=", 1);
}

char *engine_xmld_get_curr_attribute_value(XMLDFile *file) {
 int tok = dmstrchr((FILE *) file->data, "\"'", 2);
 if (tok == -1) {
  return NULL;
 }
 else if (tok == 0) {
  return dmcstrchr((FILE *) file->data, "\"", 1);
 }
 else {
  return dmcstrchr((FILE *) file->data, "'", 1);
 }
}
