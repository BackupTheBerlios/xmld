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
typedef void ConnectorData;

/* Connector capability flags */
#define CONNECTOR_NONBLOCK 0

struct Connector {
 CfgSection *cfg;
 ConnectorData *data;
 int modcap;
 
 Status (*init) (struct Connector *, void (*) (struct Connector *, int), void (*) (struct Connector *, int), void (*) (UserData *));
 Status (*set_connection_handler) (struct Connector *, void (*) (struct Connector *, int));
 Status (*set_request_handler) (struct Connector *, void (*) (struct Connector *, int));
 void (*set_user_data_free_func) (struct Connector *, void (*) (UserData *));
 Status (*add_listener) (struct Connector *, int);
 Status (*remove_listener) (struct Connector *, int);
 Status (*add_client) (struct Connector *, UserData *, int);
 Status (*remove_client) (struct Connector *, int);
 Status (*set_client_data) (struct Connector *, UserData *, int);
 UserData *(*get_client_data) (struct Connector *, int);
 Status (*run) (struct Connector *);
 Status (*stop) (struct Connector *);
 void (*destroy) (struct Connector *);
 
 Error *(*get_error) (struct Connector *);
};

typedef struct Connector Connector;

#endif /* __CONNECTOR_H */
