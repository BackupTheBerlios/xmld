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
#include "engine_xmld/element_op.h"
#include "protoimpl.h"
#include "authman.h"

/*
 * Handles an authentication sequence which
 * contains a check for existence of the user name
 * and correctness of the provided password and
 * retrieval of the base directory of the user.
 * It also involves handling DISCONNECT requests.
 * Upon successful return, info[0] is the user's name
 * and info[1] is his base_dir. The function works
 * on the given fd, and returns a success/failure
 * indiciator.
 * Note: info is a preallocated size 2 array of 
 * strings.
 */
XMLDStatus authman_handle(int fd, char **info) {
 /* Get the user name */
 char *curr_msg=protoimpl_read_sequence(fd, NULL);
 
 if (strcmp(curr_msg, DISCONNECTION_MESSAGE) == 0) {
  free(curr_msg);
  return XMLD_FAILURE;
 }
 
 char *arg_carry[1];
 char *val_carry[1];
 char *user;
 int num_parsed=protoimpl_parse_msg(curr_msg, arg_carry, val_carry, 1);
 free(curr_msg);

 if (num_parsed == 0) {
  return XMLD_FAILURE;
 }

 if (strcmp(arg_carry[0], AUTH_USER_HEADER_FIELD) != 0) {
  free(arg_carry[0]);
  free(val_carry[0]);
  return XMLD_FAILURE;
 }
 user=val_carry[0];
 free(arg_carry[0]);
 
 /* Check for user availability and get his pass and base dir */
 if (authman_auth_user(user, info) == XMLD_FAILURE) {
  free(user);
  if (protoimpl_write_sequence(fd, NULL, 0) == XMLD_FAILURE) {
   return XMLD_FAILURE;
  }
  return authman_handle(fd, info);
 }
 
 /* Inform the client that the user was found */
 if (protoimpl_write_sequence(fd, NULL, 1) == XMLD_FAILURE) {
  free(user);
  free(info[0]);
  free(info[1]);
  return XMLD_FAILURE;
 }
 
 /* Start of password checking */
 curr_msg=protoimpl_read_sequence(fd, NULL);
 
 if (strcmp(curr_msg, DISCONNECTION_MESSAGE) == 0) {
  free(curr_msg);
  free(info[0]);
  free(info[1]);
  free(user);
  return XMLD_FAILURE;
 }

 num_parsed=protoimpl_parse_msg(curr_msg, arg_carry, val_carry, 1);
 free(curr_msg);

 if (num_parsed == 0) {
  free(info[0]);
  free(info[1]);
  free(user);
  return XMLD_FAILURE;
 }

 if (strcmp(arg_carry[0], AUTH_PASS_HEADER_FIELD) != 0) {
  free(info[0]);
  free(info[1]);
  free(arg_carry[0]);
  free(val_carry[0]);
  free(user);
  return XMLD_FAILURE;
 }
 free(arg_carry[0]);
 
 /* Check for password correctness */
 if (strcmp(val_carry[0], info[0]) != 0) {
  free(user);
  free(info[0]);
  free(info[1]);
  free(val_carry[0]);
  if (protoimpl_write_sequence(fd, NULL, 0) == XMLD_FAILURE) {
   return XMLD_FAILURE;
  }
  return authman_handle(fd, info);
 }
 free(val_carry[0]);
 
 /* Inform the client that the password was correct */
 if (protoimpl_write_sequence(fd, NULL, 1) == XMLD_FAILURE) {
  free(info[0]);
  free(info[1]);
  free(user);
  return XMLD_FAILURE;
 }

 free(info[0]);
 info[0]=user;
 
 /* Finally -- Success! */
 return XMLD_SUCCESS;
}

/*
 * Returns whether user is existent. and fills
 * ret[0] with his password and ret[1] with his
 * start directory only if ret is not NULL.
 */ 
XMLDStatus authman_auth_user(char *user, char **ret) {
 FILE *auth=fopen(AUTH_FILE, "r");
 char *token[1]={AUTH_TAG};
 int tok;
 
 while (1) {
  tok=dmstrstr(auth, token, 1);
  if (tok == -1) {
   return XMLD_FAILURE;
  }
  else if (tok == 0) {
   engine_xmld_locate_att(auth, AUTH_USER_NAME);
   char *name=engine_xmld_get_curr_att_value(auth);
   if (strcmp(name, user) == 0) {
    free(name);
    if (ret != NULL) {
     engine_xmld_locate_att(auth, AUTH_USER_PASS);
     ret[0]=engine_xmld_get_curr_att_value(auth);
     engine_xmld_locate_att(auth, AUTH_USER_DIR);
     ret[1]=engine_xmld_get_curr_att_value(auth);
    } 
    return XMLD_SUCCESS;
   }
   free(name);
  }
 }
}

/*
 * Gets privileges for user over file.
 * Returns a bitmask.
 */
int authman_get_priv(char *user, char *file) {
 return XMLD_PRIV_READ|XMLD_PRIV_WRITE;
}
