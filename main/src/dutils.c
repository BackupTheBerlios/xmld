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
#define BUFFER_SIZE 100
#include "dutils.h"

/*
 * A buffered version of the getc call.
 * buf: the buffer information object which
 * gets initialized by the user function.
 */ 
char getc_buf(FILE *fd, buf_t *buf) {
 if (buf == NULL) {
  buff=(buf_t *) malloc(sizeof(buf_t));
  buff->val=fread((void *) buff->val, sizeof(char), BUFFER_SIZE, fd);
  buff->val[100]='\0';
  buff->curr=buff->val;
 }
 else if (buff->curr == (buff->val)+BUFFER_SIZE) {
  buff->val=fread((void *) buff->val, sizeof(char), BUFFER_SIZE, fd);
  buff->curr=buff->val;
 }
 else {
  buff->curr++;
 }
 return buff->curr;
}

/*
 * Dumps the buffer contained in buf
 * such that if there are characters
 * not read in it they are returned to 
 * the stream and the actual stream pointer
 * is pointed to the last character returned
 * by getc_buf on the same buf and fd.
 */ 
void buf_dump(FILE *fd, buf_t *buf) {
 char *last=buff->curr;
 while (last != '\0' && last != EOF) {
  last++;
 }
 fseek(fd, buff->curr -  last, SEEK_CUR);
 free(buf);
}

/*
 * d - disk
 * m - multi
 * dmstrstr - A multi-token on-disk version
 * of the strstr function (string.h).
 *
 * num: the number of given tokens.
 * returns: the caught token's index in the
 * tokens argument. (starting with 0)
 */ 
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

/*
 * d - disk
 * m -  multi
 * dmstrchr - A multi-token on-disk version of the
 * strchr function (string.h).
 * returns: the caught token's index in the tokens
 * argument. (starting with 0).
 */ 
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

/*
 * d - disk
 * m - multi
 * c - capture
 * dmcstrchr - A multi-token on-disk version
 * of the strchr function (string.h) with the
 * additional functionality of capturing the
 * characters from the current position to 
 * the token caught.
 * returns: the captured string.
 */ 
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

/*
 * d - disk
 * m - multi
 * w - write
 * dmwstrchr - A multi-token on-disk version
 * of the strchr function (string.h) with the
 * additional functionality of writing the
 * characters from the given write argument 
 * from the current position to the token caught.
 * it stops when it finds one of the tokens or 
 * when there are no more characters in write.
 * returns: success/failure.
 */ 
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
  fseek(fd, -1, SEEK_CUR);
  putc(*curr, fd);
  if (*(++curr) == '\0') {
   break;
  }
 }
}
