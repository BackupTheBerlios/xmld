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
