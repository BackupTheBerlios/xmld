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

#include "../includes.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../protoimpl.h"
#include "xmldclient.h"


int main(int argc, char **argv) {
 ++argv; --argc;
 short port=0;
 int fd, status, curr_char=0;
 struct conn_info info;
 char *curr_msg;
 char *query=NULL;

 if (argc > 0) {
  port=atoi(argv[0]);
 }
 
 while (1) {
  xmldclient_get_port(&port);
  if (port == 0) {
   continue;
  }
  fd=socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family=PF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  status=connect(fd, (struct sockaddr *) &addr, sizeof(addr));

  if (status == -1) {
   xmldclient_print_err("Unable to connect to the given port");
   continue;
  }

  curr_msg=protoimpl_read_sequence(fd, NULL);
  status=XMLD_SUCCESS;
  status=xmldclient_process_info_msg(curr_msg, &info);
  free(curr_msg);
  if (status == XMLD_FAILURE) {
   xmldclient_print_err("An error has occured while processing initialization message");
   continue;
  }

  status=xmldclient_auth(fd);
  if (status == XMLD_FAILURE) {
   xmldclient_print_err("Unable to authenticate user");
   continue;
  }

  while (1) {
   printf(PROMPT);
   char curr=getchar();
   while (curr != ';') {
    query=(char *) realloc(query, (curr_char+1) * sizeof(char));
    query[curr_char]=curr;
    curr_char++;
    curr=getchar();
   }
   status=protoimpl_write_sequence(fd, query, 1);
   if (status == XMLD_FAILURE) {
    xmldclient_print_err("Unable to send query");
    free(query);
    query=NULL;
    curr_char=0;
    continue;
   }
   curr_msg=protoimpl_read_sequence(fd, &status);
   if (status == XMLD_FAILURE) {
    xmldclient_print_server_err_msg(curr_msg);
    free(curr_msg);
    free(query);
    curr_char=0;
    continue;
   }
   xmldclient_print_record_set(curr_msg);
   free(curr_msg);
   free(query);
   curr_char=0;
  } 
 }
 return 0;
}

XMLDStatus xmldclient_auth(int fd) {
 return XMLD_TRUE;
}

XMLDStatus xmldclient_process_info_msg(char *msg, struct conn_info *info) {
}

void xmldclient_get_port(int *port_ptr) {
 printf("Please enter a port to which The OpenXMLD listens: ");
 scanf("%hd", port_ptr);
 printf("\n");
}

void xmldclient_print_err(char *err) {
 printf("%s.\n", err);
}

void xmldclient_print_server_err_msg(char *msg) {
}

void xmldclient_print_record_set(char *rs) {
}

