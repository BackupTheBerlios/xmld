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

/*
 * Returns the password of user or NULL
 * if such a user is non-existent.
 */ 
char *authman_auth_user(char *user) {
 FILE *auth=fopen("auth.xml", "r");
 char *token[1]={"<user"};
 int tok;
 
 while (1) {
  tok=dmstrstr(fd, token, 1);
  if (tok == -1) {
   return NULL;
  }
  else if (tok == 0) {
   engine_xmld_locate_att(auth, "name");
   char *name=engine_xmld_get_curr_att_value(auth);
   if (strcmp(name, user) == 0) {
    free(name);
    engine_xmld_locate_att(auth, "pass");
    return engine_xmld_get_curr_att_value(auth);
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
