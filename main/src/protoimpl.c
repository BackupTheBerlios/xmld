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
#include "sutils.h"

/*
 * Parses a given message header and returns whether successful.
 * and fills ret[0] with message-length and ret[1] with status.
 */
XMLDStatus protoimpl_parse_header(char *header, int **ret) {
 char *ptr[2]; /* ptr[0] points at " " and ptr[1] points at \n */
 ptr[1]=header;
 int index=0;
 
 while (index <= 1) {
  ptr[0]=strchr(ptr[1], " ");
  if (ptr[0] == NULL) {
   return XMLD_FAILURE;
  }
  ptr[1]=strchr(++ptr[0], "\n");
  if (ptr[1] == NULL) {
   return XMLD_FAILURE;
  }
  *ptr[1]='\0';
  ret[index]=atoi(ptr[0]);
  *ptr[1]='\n';
  index++;
 }
 
 return XMLD_SUCCESS;
}

/*
 * Composes a header from a string array exactly the same as that returned by 
 * protoimpl_parse_header with a maximum length of len. NULL is returned in 
 * case of error or if the composed message was larger than len.
 */
char *protoimpl_create_header(int **data, int len) {
 char *msglen_field=itostr(data[0], len - 26);
 char *status_field=(data[1][0] == '1') ? "1" : "0";
 
 int msglen=strlen(msglen_field)+26; /* 26 is length of "message-length", "status"
                                      * , two spaces, two \n's and status_field
				      * value which is a single character and a
				      * \0
			              */
 
 if (msglen > len) {
  return NULL;
 }
 
 char *ret=(char *) malloc(msglen*sizeof(char));
 strcpy(ret, "message-length ");
 strcat(ret, msglen_field);
 strcat(ret, "\n");
 strcat(ret, "status ");
 strcat(ret, status_field);
 strcat(ret, "\n");
 free(msglen_field);
 return ret;
}

/*
 * Parses a given error message -- returns the error code, and
 * fills ret[0] with information about the error.
 */
int protoimpl_parse_err_msg(char *msg, char **ret) {
 char *ptr[2];
 ptr[1]=msg;
 int which=1;
 int err_num=-1;
 ret[0]=NULL;
 
 while(which <= 2) {
  ptr[0]=strchr(ptr[1], " ");
  if (ptr[0] == NULL) {
   break;
  }
  ptr[1]=strchr(++ptr[0], "\n");
  if (ptr[1] == NULL) {
   break;
  }
  *ptr[1]='\0';
  switch (which) {
   case 1:
    err_num=atoi(ptr[0]);
   break;
   case 2:
    ret[0]=(char *) malloc((strlen(ptr[0])+1)*sizeof(char));
    strcpy(ret[0], ptr[0]);    
   break;
  }
  *ptr[1]='\n';
  which++;
 }
 
 return err_num;
}

/*
 * Composes an error message out of a given error code and
 * an error information string.
 */
char *protoimpl_create_err_msg(int code, char *inf) {
 char *code_str=itostr(code, 0);
 char *ret=(char *) malloc(strlen(code_str)+strlen(inf)+11); /* 11 - length of "MSG", "ERR" two \n and
							      * two spaces and a \0
							      */
 strcpy(ret, "ERR ");
 strcat(ret, code_str);
 strcat(ret, "\n");
 strcat(ret, "MSG ");
 strcat(ret, inf);
 strcat(ret, "\n");
 return ret;
}

/*
 * Parses a user-name message and returns the user name inside
 * it.
 */
char *protoimpl_parse_user_msg(char *msg) {
 char *ptr=strchr(msg, " ");
 if (ptr == NULL) {
  return NULL;
 }
 ptr++;
 char *ret=(char *) malloc((strlen(ptr)+1)*sizeof(char));
 strcpy(ret, ptr);
 return ret;
}

/*
 * Composes a user message and returns it.
 */
char *protoimpl_create_user_msg(char *user) {
 char *ret=(char *) malloc((strlen(user)+12)*sizeof(char)); /* 12 is length of "user-name", one
							     * space, one \n and a \0 
							     */
 strcpy(ret, "user-name ");
 strcat(ret, user);
 strcat(ret, "\n");
 return ret;
}

/*
 * Parses a pass message and returns the password inside
 * it.
 */
char *protoimpl_parse_pass_msg(char *msg) {
 char *ptr=strchr(msg, " ");
 if (ptr == NULL) {
  return NULL;
 }
 ptr++;
 char *ret=(char *) malloc((strlen(ptr)+1)*sizeof(char));
 strcpy(ret, ptr);
 return ret;
}

/*
 * Composes a password message and returns it.
 */
char *protoimpl_create_user_msg(char *pass) {
 char *ret=(char *) malloc((strlen(pass)+7)*sizeof(char)); /* 7 is length of "pass", one
							     * space, one \n and a \0 
							     */
 strcpy(ret, "pass ");
 strcat(ret, pass);
 strcat(ret, "\n");
 return ret;
}
