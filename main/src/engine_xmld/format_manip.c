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
#include "../dutils.h"
#include "../sutils.h"
#include "../fmanager.h"
#include "format_manip.h"

/*
 * : Opens the format document of the given full_file
 * in the given fd and returns whether successful.
 * fd: The file descriptor with which the opened format
 * file is to be associated.
 * full_file: The name of the file whose format file is to
 * be opened.
 */
short engine_xmld_load_format_file(FILE *fd, char *full_file, short type) {
 full_file=(char *) realloc(full_file, (strlen(full_file)+8)*sizeof(char));
 strcat(full_file, ".format");
 if (type == 12) {/* FIXME: add query types that require ex to .format */
  fd=fmanager_get_ex_fd(full_file);
 }
 else {
  fd=fmanager_get_sh_fd(full_file);
 }

 if (fd == NULL) {
  return 0;
 }
 else {
  return 1;
 }
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
 char *tokens[]={"<", "/>", "</"};
 int curr_level=0;
 while (1) {
  int tok=dmstrstr(fd, tokens, 3);
  if (tok == -1) {
   return 0;
  }
  else if (tok == 0) {
   curr_level++;
  }
  else if (tok == 1 || tok == 2) {
   curr_level--;
  }
  if (curr_level == level) {
   char *tname=str_prepend(tagname, "<");
   char *tokens2[1];
   tokens2[0]=tname;
   dmstrstr(fd, tokens2, 1);
   free(tname);
   engine_xmld_locate_att(fd, attribute);
   char *len=engine_xmld_get_curr_att_value(fd);
   int ret=sprintf(len, "%d", &ret);
   free(len);
   return ret;
  }
 } 
}

int engine_xmld_get_element_text_length(FILE *fd, int level, char *tagname) {
 return engine_xmld_get_element_text_length(fd, level, tagname, "(text)");
}
