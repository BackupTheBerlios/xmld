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

char *engine_xmld_get_curr_att_name(FILE *fd) {
 return dmcstrchr(fd, "=", 1);
}

char *engine_xmld_get_curr_att_value(FILE *fd) {
 char tokens[1];
 tokens[0]=fgetc(fd);
 return dmcstrchr(fd, tokens, 1);
}

short engine_xmld_set_curr_att_value(FILE *fd, char *value) {
 char tokens[1];
 tokens[0]=fgetc(fd);
 return dmwstrchr(fd, tokens, 1, value);
}

void engine_xmld_discard_curr_att_value(FILE *fd) {
 char tokens[1];
 tokens[0]=fgetc(fd);
 dmstrchr(fd, tokens, 1);
}

char *engine_xmld_get_text_value(FILE *fd) {
 return dmcstrchr(fd, "<", 1, value);
}

short engine_xmld_set_text_value(FILE *fd, char *value) {
 return dmwstrchr(fd, "<", 1, value);
}

char *engine_xmld_get_tagname(FILE *fd) {
 return dmcstrchr(fd, " ", 1);
}

void engine_xmld_discard_curr_att_name(FILE *fd) {
 dmstrchr(fd, "=", 1);
}

short engine_xmld_locate_text(FILE *fd) {
 char *tok[]={"/>", ">"};
 int token=dmstrstr(fd, tok, 2);
 if (token == 0) {
  return 0;
 }
 else if (token == 1) {
  return 1;
 }
}

short engine_xmld_locate_att(FILE *fd, char *col_name) {
 int token;
 while (1) {
  token=dmstrchr(fd, " >", 2);
  if (token == 1) { /* Attribute not found */
   return 0;
  }
  else if (token == 0) {
   char *att_name=engine_xmld_get_curr_att_name(fd);
   if (strcmp(att_name, col_name) == 0) {
    free(att_name);
    return 1;
   }
   else {
    engine_xmld_discard_curr_att_value(fd);
   }
  } 
 }
}
