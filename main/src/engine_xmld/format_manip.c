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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../xmlddef.h"
#include "../dutils.h"
#include "../sutils.h"
#include "../fmanager.h"
#include "element_op.h"
#include "format_manip.h"

/*
 * : Opens the format document of the given full_file
 * in the given fd and returns whether successful.
 * fd: The file descriptor with which the opened format
 * file is to be associated.
 * full_file: The name of the file whose format file is to
 * be opened.
 * ex: execlusive access flag.
 */
FILE *engine_xmld_load_format_file(char *full_file, XMLDBool ex) {
 full_file=(char *) realloc(full_file, (strlen(full_file)+8)*sizeof(char));
 strcat(full_file, ".format");
 FILE *fd;

 if (ex == XMLD_FALSE) {
  fd=fmanager_get_sh_fd(full_file);
 }
 else {
  fd=fmanager_get_ex_fd(full_file);
 }

 return fd; 
}

/*
 * : Gets the length of an attribue of an element with
 * a specefied name in a sepcified level in a document
 * according to its format document.
 * fd: The file descriptor of the format document.
 * level: The level of the element in the original document
 * to which the attribute whose length is in question belongs.
 * tagname: The tag name of the element in the original document
 * to which the attribue whose length is in question belongs.
 * attribute: The name of the attribute whose length is to be
 * returned.
 */
int engine_xmld_get_element_att_length(FILE *fd, int level, char *tagname, char *attribute) {
 char *format=engine_xmld_get_element_att_format(fd, level, tagname, attribute);
 if (format == NULL) {
  return 0;
 }
 char *stroke=strchr(format, '|');
 *stroke='\0';
 int ret;
 sscanf(format, "%d", &ret);
 free(format);
 return ret;
}

/*
 * Gets the type of an attribute.
 */ 
char *engine_xmld_get_element_att_type(FILE *fd, int level, char *tagname, char *attribute) {
 char *format=engine_xmld_get_element_att_format(fd, level, tagname, attribute);
 if (format == NULL) {
  return NULL;
 } 
 char *stroke=strchr(format, '|');
 if (stroke == NULL) {
  return NULL;
 }
 stroke++;
 char *ret=(char *) malloc((strlen(stroke)+1) * sizeof(char));
 strcpy(ret, stroke);
 free(format);
 return ret;
}

/*
 * Gets the attribute barely. Other functions use either the type
 * or the length. (attribute form: attname="length|type").
 */ 
char *engine_xmld_get_element_att_format(FILE *fd, int level, char *tagname, char *attribute) {
 rewind(fd);
 char *tokens[]={"<level>", "</level>"};
 int curr_level=0;
 XMLDBool proceed=XMLD_FALSE;
 fpos_t pos;
 int tok;
 
 while (1) {
  fgetpos(fd, &pos);
  tok=dmstrstr(fd, tokens, 2);
  if (tok == -1) {
   return NULL;
  }
  else if (tok == 0) {
   curr_level++;
   if (curr_level == level) {
    proceed = XMLD_TRUE;
   }
  }
  else if (tok == 1) {
   fsetpos(fd, &pos);
   proceed = XMLD_TRUE;
  }
  if (proceed == XMLD_TRUE) {
   char *tname=str_prepend(tagname, "<");
   char *tokens2[2];
   tokens2[0]=tname;
   tokens2[1]="</level>";
   tok=dmstrstr(fd, tokens2, 2);
   if (tok == 0) {
    free(tname);
    if (engine_xmld_locate_att(fd, attribute)) {
     return engine_xmld_get_curr_att_value(fd);
    }
    else {
     return NULL;
    }
   }
   else {
    fsetpos(fd, &pos);
    free(tname);
    tname=str_prepend("!", "<");
    tokens2[0]=tname;
    tok=dmstrstr(fd, tokens2, 2);
    if (tok == 0) {
     free(tname);
     if (engine_xmld_locate_att(fd, attribute)) {
      return engine_xmld_get_curr_att_value(fd);
     }
     else {
      return NULL;
     }
    }
    else {
     free(tname);
     return NULL;
    }
   }
  }
 } 
}
