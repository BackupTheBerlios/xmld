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
 * Returns whether user is existent. and fills
 * ret[0] with his password and ret[1] with his
 * start directory only if ret is not NULL.
 */ 
XMLDStatus authman_auth_user(char *user, char **ret) {
 FILE *auth=fopen("auth.xml", "r");
 char *token[1]={"<user"};
 int tok;
 
 while (1) {
  tok=dmstrstr(fd, token, 1);
  if (tok == -1) {
   return XMLD_FAILURE;
  }
  else if (tok == 0) {
   engine_xmld_locate_att(auth, "name");
   char *name=engine_xmld_get_curr_att_value(auth);
   if (strcmp(name, user) == 0) {
    free(name);
    engine_xmld_locate_att(auth, "pass");
    ret[0]=engine_xmld_get_curr_att_value(auth);
    engine_xmld_locate_att(auth, "dir");
    ret[1]=engine_xmld_get_curr_att_value(auth);
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
