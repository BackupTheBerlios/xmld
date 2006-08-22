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

#ifndef __MCET_H
#define __MCET_H

/* OpenDaemon symbols */
char opendaemon_version[] = "0.8.0";
void *_get_module_instance(CfgSection *);

/* Internal symbols */
struct MCETData {
 thread_pool *executor_pool;
 Assoc *socks;
 Assoc *errors;
 fd_set tmp_set;
 int n;
 void (*conn_handler) (Connector *, int);
 void (*req_handler) (Connector *, int);
 void (*user_data_free_func) (UserData *);
 Bool infinite_run;
 Bool intact_fd_set;
 Bool stop;
 int run_iterations;
 int timeout_seconds;
 int timeout_microseconds;
};

typedef struct MCETData MCETData;
 
Status mcet_init(Connector *, void (*) (Connector *, int), void (*) (Connector *, int), void (*) (UserData *));
Status mcet_set_connection_handler (Connector *, void (*) (Connector *, int));
Status mcet_set_request_handler (Connector *, void (*) (Connector *, int));
void mcet_set_user_data_free_func (Connector *, void (*) (UserData *));
Status mcet_add_listener (Connector *, int);
Status mcet_remove_listener (Connector *, int);
Status mcet_add_client (Connector *, UserData *, int);
Status mcet_remove_client (Connector *, int);
Status mcet_run (Connector *);
Status mcet_stop(Connector *);
Status mcet_set_client_data(Connector *, UserData *, int);
UserData *mcet_get_client_data(Connector *, int);
void mcet_destroy (Connector *);
Error *mcet_get_error(Connector *);

/* Internal declarations */
void _req_handler(void *);
void _conn_handler(void *);
void _add_error(Connector *, char *, ErrorLevel);
void _remove_error(Connector *);


#endif /* __MCET_H */
