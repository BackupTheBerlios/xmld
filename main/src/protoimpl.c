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
#include "xmld_sockets.h"
#include "sutils.h"
#include "protoimpl.h"

/*
 * Composes a message in the form:
 *
 * <argument> <value>
 *
 * arguments are taken from args and values are taken from vals
 * such that each value is associated with the argument with the
 * same index in args. The generated message is never longer than
 * len--arguments that go past it are ignored. This method returns 
 * the composed message. If len was 0, it's ignored.
 */
char *protoimpl_compose_msg(char **args, char **vals, int num, int len) {
 char *ret=(char *) malloc(sizeof(char));
 ret[0]='\0';
 int i=0, ret_len=1;

 while (i < num) {
  ret_len += strlen(args[i]) + strlen(vals[i]) + 2; /* 2 is \n + space */
  if (ret_len > len && len != 0) {
   break;
  }
  ret=(char *) realloc(ret, ret_len * sizeof(char));
  strcat(ret, args[i]);
  strcat(ret, " ");
  strcat(ret, vals[i]);
  strcat(ret, "\n");
  i++;
 }

 return ret;
}

/*
 * Parses a message in the form above and stores its arguments in the
 * string array args and their values in the corresponding indeces
 * in the vals array, returns the number of arguments processed and 
 * proccesses only a maximum of num arguments. Note that args and
 * vals are pre-allocated and must not be changed by the function.
 */
int protoimpl_parse_msg(char *msg, char **args, char **vals, int num) {
 char *ptr[2]; /* 0 -> ' ', 1 -> '\n' */
 ptr[1]=msg;
 int num_taken=0;
 
 while (num_taken < num) {
  ptr[0]=strchr(msg, ' ');
  if (ptr[0] == NULL) {
   break;
  }
  *ptr[0]='\0';
  args[num_taken]=(char *) malloc((strlen(ptr[1])+1) * sizeof(char));
  strcpy(args[num_taken], ptr[1]);
  *ptr[0]=' ';
  ptr[1]=strchr(ptr[0], '\n');
  if (ptr[1] == NULL) {
   free(args[num_taken]);
   break;
  }
  ptr[0]++;
  ptr[1]='\0';
  vals[num_taken]=(char *) malloc((strlen(ptr[0])+1) * sizeof(char));
  strcpy(vals[num_taken], ptr[0]);
  *ptr[1]='\n';
  ptr[1]++;
  num_taken++;
 }
 
 return num_taken;
}

/*
 * Composes a header in the form:
 *
 * message-length <length>
 * status <binary value>
 *
 * the given len and status arguments fill message-length and status fields
 * respectively. Upon successful composition the created header message is
 * returned -- NULL is returned otherwise.
 */
char *protoimpl_compose_header(int len, int status) {
 char *arg_carry[HEADER_FIELDS]={HEADER_MESSAGE_LENGTH_ARG, HEADER_STATUS_ARG};
 char *val_carry[HEADER_FIELDS];
 val_carry[0]=itostr(len, HEADER_MESSAGE_LENGTH_LEN);
 val_carry[1]=(status == 0) ? "0" : "1";
 
 char *curr_msg=protoimpl_compose_msg(arg_carry, val_carry, HEADER_FIELDS, HEADER_LENGTH);
 free(val_carry[0]);
 return curr_msg;
}

/*
 * Parses a header in the form above and returns a success/failure indiciator
 * the locations pointed by len and status are filled with the header's message
 * -length and status fields' values respectively only if they were not provided
 * as NULL.
 */
XMLDStatus protoimpl_parse_header(char *header, int *len, int *status) {
 int num_parsed;
 char *arg_carry[HEADER_FIELDS];
 char *val_carry[HEADER_FIELDS];
 num_parsed=protoimpl_parse_msg(header, arg_carry, val_carry, 2);
 
 if (num_parsed < 2) {
  if (num_parsed == 1) {
   free(val_carry[0]);
   free(arg_carry[0]);
  }
  return XMLD_FAILURE;
 }
 
 if (strcmp(arg_carry[0], HEADER_MESSAGE_LENGTH_ARG) != 0 ||
     strcmp(arg_carry[1], HEADER_STATUS_ARG) != 0) {
  free(val_carry[0]);
  free(arg_carry[0]);
  free(val_carry[1]);
  free(arg_carry[1]);
  return XMLD_FAILURE;
 }

 if (len != NULL) {
  *len=atoi(val_carry[0]);
 } 
 if (status != NULL) {
  *status=atoi(val_carry[1]);
 } 
 
 free(val_carry[0]);
 free(val_carry[1]);
 free(arg_carry[0]);
 free(arg_carry[1]);

 return XMLD_SUCCESS;
}

/*
 * Reads a sequence of a header + message from fd - returns the message itself
 * or NULL if an error occurs, on success the header status is put in the 
 * location pointed by status only if it was not NULL.
 */
char *protoimpl_read_sequence(int fd, int *status) {
 char *header=xmld_socket_read(fd, HEADER_LENGTH);
 int msg_len;
 
 if (protoimpl_parse_header(header, &msg_len, status) == XMLD_FAILURE) {
  free(header);
  return NULL;
 }
 free(header);
 return xmld_socket_read(fd, msg_len);
}

/*
 * Writes a sequence of a header + message to fd - returns success/failure and 
 * writes a message whose contents are the of msg and sets the header's status
 * to the provided stat argument.
 */
XMLDStatus protoimpl_write_sequence(int fd, char *msg, int stat) {
 int msg_len=(msg == NULL) ? 0 : strlen(msg)+1;
 char *curr_msg=protoimpl_compose_header(msg_len, stat);
 
 if (curr_msg == NULL) {
  return XMLD_FAILURE;
 }
 
 xmld_socket_write(fd, curr_msg, HEADER_LENGTH);
 
 if (msg != NULL) {
  xmld_socket_write(fd, msg, msg_len);
 } 
 
 free(curr_msg);
 return XMLD_SUCCESS;
}
