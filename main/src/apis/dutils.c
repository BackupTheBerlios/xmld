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
#include "xmlddef.h"
#include "dutils.h"

/*
 * Creates a new buffer object.
 */
buf_t *buf_create() {
 buf_t *ret=(buf_t *) malloc(sizeof(buf_t));
 ret->val[0]='\0';
 ret->curr=ret->val;
 return ret;
}

/*
 * A buffered version of the getc call.
 * buf: the buffer information object which
 * gets initialized by the user.
 */ 
char getc_buf(FILE *fd, buf_t *buf) {
 if (*buf->curr == '\0') {
  int bytes_read=fread(buf->val, sizeof(char), BUFFER_SIZE-1, fd);
  if (bytes_read == 0) {
   return EOF;
  }
  else {
   buf->curr=buf->val;
   buf->val[bytes_read]='\0';
  }
 }
 else {
  buf->curr++;
 }
 return *buf->curr;
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
 char *null_char=strchr(buf->val, '\0');
 fseek(fd, buf->curr - null_char + 1, SEEK_CUR);
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
 buf_t *buf=buf_create();
 
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
    if (cur[i] == strlen(tokens[i])) {
     free(cur);
     buf_dump(fd, buf);
     return i;
    }
   }
   else {
    cur[i]=0;
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
 buf_t *buf=buf_create();

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
 buf_t *buf=buf_create();
 char *cap;
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
  cap=(char *) realloc(cap, (strlen(cap)+2)*sizeof(char));
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
XMLDStatus dmwstrchr(FILE *fd, char *tokens, int num, char *write) {
 char c;
 int i;
 char *curr=write;
 
 while (1) {
  c=getc(fd);
  if (c == EOF) {
   return XMLD_FAILURE;
  }
  for (i = 0; i < num; i++) {
   if (c == tokens[i]) {
    return XMLD_SUCCESS;
   }
  }
  fseek(fd, -1, SEEK_CUR);
  putc(*curr, fd);
  if (*(++curr) == '\0') {
   return XMLD_SUCCESS;
  }
 }
}
