/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                     *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#ifndef __AUTH_H
#define __AUTH_H

typedef void AuthData;

/* Capabilities */
#define AUTHCAP_AUTHN 2
#define AUTHCAP_AUTHZ 4

struct Auth {
 int auth_cap;
 CfgTree *cfg;
 AuthData *data;
 
 /* Authentication */
 Status (*authenticate) (Auth *, char *);
 Status (*add_user) (Auth *, char *, char *);
 Status (*remove_user) (Auth *, char *);
 Status (*update_user) (Auth *, char *, char *);

 /* Autherization */
 Status (*add_resource) (Auth *, char *);
 Status (*remove_resource) (Auth *, char *);
 int (*get_privileges) (Auth *, char *, char *);
 Status (*update_privileges) (Auth *, char *, char *, int);

 /* Error Reporting */
 Error *(*get_error) (Auth *);
}

typedef struct Auth Auth;

#endif /* __AUTH_H */
