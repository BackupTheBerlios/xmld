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
#include "dutils.h"
#define BUFFER_SIZE 100

char getc_buf(FILE *fd, buf_t *buf) {
 return getc(fd);
}

void buf_dump(FILE *fd, buf_t *buf) { 
}

int dmstrstr(FILE *fd, char **tokens, int num) {
 int *cur=(int *) calloc(num, sizeof(int));
 char c;
 int i;
 buf_t *buf=NULL;

 while (1) {
  c=getc_buf(fd, buf);
  if (c == EOF) {
   buf_dump(fd, buf);
   free(cur);
   return -1;
  }
  for (i = 0; i < num; i++) {
   if (c == tokens[i][cur[i]]) {
    cur[i]++;
    if (cur[i] == strlen(tokens[i])+1) {
     free(cur);
     buf_dump(fd, buf);
     return i;
    }
   }
  }
 }
}

int dmstrchr(FILE *fd, char *tokens, int num) {
 char c;
 int i;
 buf_t *buf=NULL;

 while (1) {
  c=getc_buf(fd, buf);
  if (c == EOF) {
   buf_dump(fd, buf);
   return -1;
  }
  for (i = 0; i < num; i++) {
   if (c == tokens[i]) {
    buf_dump(fd, buf);
    return i;
   }
  }
 }
}

char *dmcstrchr(FILE *fd, char *tokens, int num) {
 char c;
 int i;
 buf_t *buf=NULL;
 char *cap;
 char *curr;
 cap=(char *) malloc(sizeof(char));
 cap[0]='\0';

 while (1) {
  c=getc_buf(fd, buf);
  if (c == EOF) {
   buf_dump(fd, buf);
   free(cap);
   return NULL;
  }
  for (i = 0; i < num; i++) {
   if (c == tokens[i]) {
    buf_dump(fd, buf);
    return cap;
   }
  }
  cap=(char *) realloc((strlen(cap)+2)*sizeof(char));
  cap[strlen(cap)+1]='\0';
  cap[strlen(cap)]=c;
 }
}

short dmwstrchr(FILE *fd, char *tokens, int num, char *write) {
 char c;
 int i;
 char *curr=write;
 
 while (1) {
  c=getc(fd);
  if (c == EOF) {
   return 0;
  }
  for (i = 0; i < num; i++) {
   if (c == tokens[i]) {
    return 1;
   }
  }
  ungetc(fd);
  putc(*curr, fd);
  if (*(++curr) == '\0') {
   break;
  }
 }
}
