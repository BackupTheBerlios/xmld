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

#ifndef __COMMUNICATOR_H
#define __COMMUNICATOR_H

/* send_message() flags */
#define MSG_LISTENER 0
#define MSG_CHANNEL 1

typedef void CommunicatorData;
typedef void Message;

struct Communicator {
 CfgSection *cfg;
 CommunicatorData *data;
 
 void (*init) (Communicator *, void (*) (char *));
 Status (*create_channel) (Communicator *, char *);
 Status (*destroy_channel) (Communicator *, char *);
 Status (*create_listener) (Communicator *, char *);
 Status (*destroy_listener) (Communicator *, char *);
 Status (*bind_listener) (Communicator *, char *, char *);
 Status (*unbind_listener) (Communicator *, char *, char *);
 Status (*send_message) (Communicator *, Message *, char *, int);
 Message *(*recv_message) (Communicator *, char *);
 
 Status (*set_message_handler) (Communicator *, void (*) (char *));
 void (*destroy) (Communicator *);

 Error *(*get_error) (Communicator *);
}

#endif /* __COMMUNICATOR_H */
