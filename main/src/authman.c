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

#include "includes.h"
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

 if (strcmp(arg_carry[0], USER_NAME_FIELD) != 0) {
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

 if (strcmp(arg_carry[0], PASS_FIELD) != 0) {
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
 char *full_file = (char *) malloc((strlen(document_root)+strlen(user)+10) * sizeof(char));
 strcpy(full_file, document_root);
 strcat(full_file, "xmld/");
 strcat(full_file, user);
 strcat(full_file, ".xml");

 XMLDFile *auth = XMLDFile_create(NULL);
 if (auth->data == NULL) {
  return XMLD_FAILURE;
 }
 else {
  if (ret == NULL) {
   return XMLD_SUCCESS;
  }
 }
 
 auth->engine = XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
 (*(auth->engine->prepare)) (full_file, auth, XMLD_ACCESS_NOTHING);
 
 int walk = (*(auth->engine->walk)) (auth);
 int level = 0;
 while (walk != XMLD_WALK_END) {
  if (walk == XMLD_WALK_DOWN) {
   level++;
   if (level == AUTH_TOK_LEVEL) {
    ret[0] = (*(auth->engine->get_attribute)) (auth, AUTH_USER_PASS);
    ret[1] = (*(auth->engine->get_attribute)) (auth, AUTH_USER_DIR);
    return XMLD_SUCCESS;
   }
  }
  else if (walk == XMLD_WALK_UP) {
   level--;
  }
  walk = (*(auth->engine->walk)) (auth);
 }
 return XMLD_SUCCESS;
}

/*
 * Gets privileges for user over file.
 * Returns a bitmask.
 */
int authman_get_priv(char *user, char *file) {
 char *full_file = (char *) malloc((strlen(document_root)+strlen(user)+10) * sizeof(char));
 strcpy(full_file, document_root);
 strcat(full_file, "xmld/");
 strcat(full_file, user);
 strcat(full_file, ".xml");
 
 XMLDFile *priv = XMLDFile_create(NULL);
 priv->engine = XMLDEngineList_search_by_name(engine_list, cfg_get_engine(full_file));
 (*(priv->engine->prepare)) (full_file, priv, XMLD_ACCESS_FORMAT);
 int walk = (*(priv->engine->walk)) (priv);
 int level = 0;
 int ret;
 
 while (walk != XMLD_WALK_END) {
  if (walk == XMLD_WALK_DOWN) {
   level++;
   if (level == AUTH_PRIV_LEVEL) {
    char *path = (*(priv->engine->get_attribute)) (priv, AUTH_PATH);
    if (strcmp(path, file) == 0) {
     free(path);
     path = (*(priv->engine->get_attribute)) (priv, AUTH_PRIV);
     ret = atoi(path);
     free(path);
     return ret;
    }
    free(path);
   }
  }
  else if (walk == XMLD_WALK_UP) {
   level--;
  }
  walk = (*(priv->engine->walk)) (priv);
 }
 return XMLD_PRIV_NOTHING;
}
