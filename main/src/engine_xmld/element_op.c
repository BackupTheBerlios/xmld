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
 int factor=1; /* factor multiplied by ATTRIBUE_LENGTH if the attribute's length
  was larger than the original ATTRIBUTE_LENGTH*/
 while (1) {
  char *att_name=(char *) malloc((ATTRIBUTE_LENGTH+1)*sizeof(char));
  att_name[15]='\0';
  int num;
  fscanf(fd, "%ATTRIBUTE_LENGTH[^=]%n", att_name, &num);
  if (num < ATTRIBUTE_LENGTH) { /* it was shorter than ATTRIBUTE_LENGTH */
   att_name=(char *) realloc(att_name, (num+1)*sizeof(char));
   att_name[num]='\0';
   break;
  }
  else if (num == ATTRIBUTE_LENGTH) {
   if ((char) fgetc(fd) != '=') {
    fseek(fd, -1, SEEK_CUR);
    factor++;
   }
   else {
    break;
   }
  }
 }
}

char *engine_xmld_get_curr_att_value(FILE *fd) {
}

void engine_xmld_discard_curr_att_value(FILE *fd) {
}

char *engine_xmld_get_text_value(FILE *fd) {
}

char *engine_xmld_get_curr_tagname(FILE *fd) {
}

void engine_xmld_discard_curr_att_name(FILE *fd) {
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