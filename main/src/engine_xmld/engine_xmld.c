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
  if (engine_xmld_load_format_file(file, full_name, BIT_ISSET(level, XMLD_ACCESS_FORMAT_EX)) == XMLD_FAILURE) {
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

XMLDBool engine_xmld_is_valid_mime(char *mime) {
 return XMLD_TRUE;
}

/* load_format_file function */
XMLDStatus engine_xmld_load_format_file(XMLDFile *file, char *name, XMLDBool ex) {
 full_file=(char *) realloc(full_file, (strlen(full_file)+8)*sizeof(char));
 strcat(full_file, ".format");
 FILE *fd;
 
 if (ex == XMLD_FALSE) {
  fd=fmanager_get_sh_fd(full_file);
 }
 else {
  fd=fmanager_get_ex_fd(full_file);
 }

 if (fd != NULL) {
  file->store = XMLDFile_create();
  file->store->data = (void *) fd;
  file->store->level = 0;
  return XMLD_SUCCESS;
 }
 else {
  return XMLD_FAILURE;
 }
} 

/* cleanup function */
void engine_xmld_cleanup(XMLDFile *file) {
 if (file->store != NULL) {
  engine_xmld_unload_format_file(file);
 }
 fmanager_unlock_fd((FILE *) file->data);
 fclose((FILE *) file->data);
}

/* unload_format_file function */
void engine_xmld_unload_format_file(XMLDFile *file) {
 fmanager_unlock_fd((FILE *) file->store->data);
 fclose((FILE *) file->store->data);
 XMLDFile_free(file->store);
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
int engine_xmld_walk(XMLDFile *file) {
 int token;
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


char *engine_xmld_get_attribute(XMLDFile *file, char *attribute) {
 engine_xmld_reset_element(file);
 char *name;
 while (engine_xmld_next_attribute(file) == XMLD_TRUE) {
  name = engine_xmld_get_curr_attribute_name(file);
  if (strcmp(name, attribute) == 0) {
   free(name);
   return engine_xmld_get_curr_attribute_value(file);
  }
  free(name);
 }
}

char *_get_attribute_format(XMLDFile *file, char *attribute) {
 int ret = engine_xmld_walk(file->store);
 while (file->store->level != file->level && ret != XMLD_WALK_END) {
  ret = engine_xmld_walk(file->store);
 }
 char *format = engine_xmld_get_attribute(file->store, attribute);
 return format;
}

int _get_format_length(char *format) {
 char *stroke=strchr(format, '|');
 if (stroke == NULL) {
  return NULL;
 }
 *stroke='\0';
 int ret;
 sscanf(format, "%d", &ret);
 return ret;
}

char *_get_format_type(char *format) {
 char *stroke=strchr(format, '|');
 if (stroke == NULL) {
  return NULL;
 }
 stroke++;
 char *ret=(char *) malloc((strlen(stroke)+1) * sizeof(char));
 strcpy(ret, stroke);
 return ret;
}

char *engine_xmld_get_attribute_type(XMLDFile *file, char *attribute) {
 char *format = _get_attribute_format(file, attribute);
 if (format != NULL) {
  return _get_format_type(format);
 }
 else {
  return NULL;
 }
}

int engine_xmld_get_attribute_length(XMLDFile *file, char *attribute) {
 char *format = _get_attribute_format(file, attribute);
 if (format != NULL) {
  return _get_format_length(format);
 }
 else {
  return NULL;
 }
}

char *engine_xmld_get_text(XMLDFile *file) {
 engine_xmld_reset_element(file);
 char *tokens[2]={"/>", ">"};
 int tok = dmstrstr((FILE *) file->data, tokens, 2);
 if (tok == -1 || tok == 0) {
  return NULL;
 }
 else {
  return dmcstrchr((FILE *) file->data, "<", 1);
 } 
}

char *_get_text_format(XMLDFile *file) {
 int ret = engine_xmld_walk(file->store);
 while (file->store->level != file->level && ret != XMLD_WALK_END) {
  ret = engine_xmld_walk(file->store);
 }
 char *format = engine_xmld_get_text(file->store);
 return format;
}

char *engine_xmld_get_text_type(XMLDFile *file) {
 char *format = _get_text_format(file);
 if (format != NULL) {
  return _get_format_type(format);
 }
 else {
  return NULL;
 }
}

int engine_xmld_get_text_length(XMLDFile *file) {
 char *format = _get_text_format(file);
 if (format != NULL) {
  return _get_format_length(format);
 }
 else {
  return NULL;
 }
}

char *engine_xmld_get_tagname(XMLDFile *file) {
 engine_xmld_reset_element(file);
 return dmcstrchr((FILE *) file->data, " />", 3);
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

int engine_xmld_get_curr_attribute_length(XMLDFile *file) {
 char *att_name = engine_xmld_get_curr_attribute_name(file);
 int len = engine_xmld_get_attribute_length(file, att_name);
 free(att_name);
 return len;
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
