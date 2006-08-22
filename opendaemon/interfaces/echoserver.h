/*                                                                *
 * -------------------------------------------------------------- *
 * OpenDaemon                                                  *
 * -------------------------------------------------------------- *
 * This source file is subject to the GNU General Public licence, *
 * which can be obtained through the world-wide-web at:           *
 *                                                                *
 *  http://www.gnu.org/copyleft/gpl.html                          *
 * -------------------------------------------------------------- *
 * Authors: Khalid Al-Kary (khalid_kary@hotmail.com)              *
 * -------------------------------------------------------------- * 
 */

#ifndef __ECHOSERVER_H
#define __ECHOSERVER_H

/* OpenDaemon Symbols */
char opendaemon_version[] = "0.8.0";
void *_get_module_instance(CfgSection *);

/* Internal Symbols */
struct EchoServerData {
 int max_msg_length;
 Assoc *errors;
 Connector *mcet;
};

typedef struct EchoServerData EchoServerData;

Status echoserver_init(Interface *);
void echoserver_main(Interface *);
void echoserver_destroy(Interface *);
void echoserver_connection(Connector *, int);
void echoserver_request(Connector *, int);
void echoserver_free_user(UserData *);
Error *echoserver_get_error(Interface *);
void _add_error(Interface *, char *, ErrorLevel);
void _remove_error(Interface *);

#endif /* __ECHOSERVER_H */
