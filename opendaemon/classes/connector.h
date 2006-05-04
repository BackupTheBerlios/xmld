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

#ifndef __CONNECTOR_H
#define __CONNECTOR_H

typedef void UserData;

/* Connector capability flags */
#define CONNECTOR_NONBLOCK 0

struct Connector {
 CfgSection *cfg;
 ConnectorData *data;
 int modcap;
 
 Status (*init) (Connector *, void (*) (Connector *, int), void (*) (Connector *, int));
 Status (*set_connection_handler) (Connector *, void (*) (Connector *, int));
 Status (*set_request_handler) (Connector *, void (*) (Connector *, int));
 Status (*set_user_data_free_func) (Connector *, void (*) (UserData *));
 Status (*add_listener) (Connector *, int);
 Status (*remove_listener) (Connector *, int);
 Status (*add_client) (Connector *, UserData *, int);
 Status (*remove_client) (Connector *, int);
 Status (*set_client_data) (Connector *, UserData, int);
 UserData *(*get_client_data) (Connector *, int);
 Status (*run) (Connector *);
 Status (*stop) (Connector *);
 void (*destroy) (Connector *);
 
 Error *(*get_error) (Connector *);
}

#endif /* __CONNECTOR_H */
