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
void *_get_module_instance(CfgTree *);

/* Internal Symbols */
struct EchoServerData {
 int max_msg_length;
 Connector *mcet;
}

Status echoserver_init(Interface *);
void echoserver_main(Interface *);
void echoserver_destroy(Interface *);
Error *echoserver_get_error(Interface *);

#endif /* __ECHOSERVER_H */
