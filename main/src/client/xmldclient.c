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
#include <signal.h>
#include "../xmld_sockets.h"
#include "../protoimpl.h"
#include "xmldclient.h"

int main_fd=0;

int main(int argc, char **argv) {
 ++argv; --argc;
 short port=0;
 int fd, status, curr_char=0;
 struct conn_info info;
 info.col_sep_enc=NULL;
 info.row_sep_enc=NULL;
 info.down_level_enc=NULL;
 info.up_level_enc=NULL;
 char *curr_msg;
 char *query=NULL;

 struct sigaction action;
 action.sa_handler=xmldclient_disconnect;
 sigaction(SIGINT, &action, NULL);
 
 if (argc > 0) {
  port=atoi(argv[0]);
 }
 
 while (port == 0) {
  xmldclient_get_port(&port);
 }
 
  fd=socket(PF_INET, SOCK_STREAM, 0);
  main_fd=fd;

 while (1) {
  struct sockaddr_in addr;
  addr.sin_family=PF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  status=connect(fd, (struct sockaddr *) &addr, sizeof(addr));

  if (status == -1) {
   xmldclient_print_err("Unable to connect to the given port");
   xmldclient_get_port(&port);
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
  status=xmldclient_auth(fd, argv, argc);
  if (status == XMLD_FAILURE) {
   xmldclient_free_info(&info);
   xmldclient_print_err("Unable to authenticate user");
   continue;
  }

  while (1) {
   printf(PROMPT);
   char curr;
   scanf("%c", &curr);
   while (curr == '\n') {
    scanf("%c", &curr);
   } 
   while (curr != ';') {
    query=(char *) realloc(query, (curr_char + 2) * sizeof(char));
    query[curr_char++]=curr;
    query[curr_char]='\0';
    scanf("%c", &curr);
   }
   status=protoimpl_write_sequence(fd, query, 1);
   free(query);
   query=NULL;
   curr_char=0;
   if (status == XMLD_FAILURE) {
    xmldclient_print_err("Unable to send query");
    continue;
   }
   curr_msg=protoimpl_read_sequence(fd, &status);
   printf("%s\n", curr_msg);
   if (status == XMLD_FAILURE) {
    xmldclient_print_server_err_msg(curr_msg);
    free(curr_msg);
    continue;
   }
   xmldclient_print_record_set(curr_msg, &info);
   free(curr_msg);
  } 
  xmldclient_free_info(&info);
 }
 return 0;
}

XMLDStatus xmldclient_auth(int fd, char **argv, int argc) {
 char *args[1]={USER_NAME_FIELD};
 char *vals[1];
 XMLDStatus stat;
 
 if (argc > 1) {
  vals[0]=argv[1];
 }
 else {
  vals[0]=NULL;
 }
 
 while (1) {
  if (vals[0] == NULL) {
   vals[0]=xmldclient_input_string("User name");
  } 
  char *msg=protoimpl_compose_msg(args, vals, 1, 0);
  free(vals[0]);
  vals[0]=NULL;
  if (protoimpl_write_sequence(fd, msg, 1) == XMLD_FAILURE) {
   free(msg);
   return XMLD_FAILURE;
  }
  free(msg);
  msg=xmld_socket_read(fd, HEADER_LENGTH);
  protoimpl_parse_header(msg, NULL, &stat);
  if (stat == XMLD_FAILURE) {
   xmldclient_print_err("Invalid user name");
   continue;
  }
  
  int num_input=0;
  while (num_input < 3) {
   num_input++;
   args[0]=PASS_FIELD;
   vals[0]=xmldclient_input_string("Password");
   msg=protoimpl_compose_msg(args, vals, 1, 0);
   free(vals[0]);
   if (protoimpl_write_sequence(fd, msg, 1) == XMLD_FAILURE) {
    free(msg);
    return XMLD_FAILURE;
   }
   free(msg);
   msg=xmld_socket_read(fd, HEADER_LENGTH);
   protoimpl_parse_header(msg, NULL, &stat);
   free(msg);
   if (stat == XMLD_FAILURE) {
    xmldclient_print_err("Invalid password");
    continue;
   }
   return XMLD_SUCCESS;
  }
 } 
 return XMLD_SUCCESS;
}

XMLDStatus xmldclient_process_info_msg(char *msg, struct conn_info *info) {
 char *curr=msg;
 char *token=NULL;
 int tok_curr=0;
 int num_init=0;
 char **fill_str=NULL;
 char *fill_char=NULL;
 while (*curr != '\0') {
  if (*curr == ' ') {
   if (token == NULL) {
    xmldclient_free_info(info);
    return XMLD_FAILURE;
   }
   else if (strcmp(token, COL_SEP_FIELD) == 0) {
    fill_char=&(info->col_sep);
    fill_str=NULL;
   }
   else if (strcmp(token, COL_SEP_ENC_FIELD) == 0) {
    fill_char=NULL;
    fill_str=&(info->col_sep_enc);
   }
   else if (strcmp(token, ROW_SEP_FIELD) == 0) {
    fill_char=&(info->row_sep);
    fill_str=NULL;
   }
   else if (strcmp(token, ROW_SEP_ENC_FIELD) == 0) {
    fill_char=NULL;
    fill_str=&(info->row_sep_enc);
   }
   else if (strcmp(token, DOWN_LEVEL_FIELD) == 0) {
    fill_char=&(info->down_level);
    fill_str=NULL;
   }
   else if (strcmp(token, DOWN_LEVEL_ENC_FIELD) == 0) {
    fill_char=NULL;
    fill_str=&(info->down_level_enc);
   }
   else if (strcmp(token, UP_LEVEL_FIELD) == 0) {
    fill_char=&(info->up_level);
    fill_str=NULL;
   }
   else if (strcmp(token, UP_LEVEL_ENC_FIELD) == 0) {
    fill_char=NULL;
    fill_str=&(info->up_level_enc);
   }
   else {
    free(token);
    token=NULL;
    tok_curr=0;
    xmldclient_free_info(info);
    return XMLD_FAILURE;
   }
   free(token);
   token=NULL;
   tok_curr=0;
  }
  else if (*curr == '\n') {
   if (token == NULL) {
    xmldclient_free_info(info);
    return XMLD_FAILURE;
   }
   else {
    if (fill_str != NULL) {
     *fill_str=(char *) malloc((strlen(token)+1)*sizeof(char));
     strcpy(*fill_str, token);
    }
    else if (fill_char != NULL) {
     sscanf(token, "%c", fill_char);
    }
    else {
     free(token);
     token=NULL;
     tok_curr=0;
     xmldclient_free_info(info);
     return XMLD_FAILURE;
    }
    free(token);
    token=NULL;
    tok_curr=0;
   }
   num_init++;
  }
  else {
   token=(char *) realloc(token, (tok_curr+2) * sizeof(char));
   token[tok_curr++]=*curr;
   token[tok_curr]='\0';
  }
  curr++;
 }
 cfree(token);
 if (num_init < INIT_NUM_FIELDS) {
  xmldclient_free_info(info);
  return XMLD_FAILURE;
 }
 else {
  return XMLD_SUCCESS;
 } 
}

void xmldclient_get_port(short *port_ptr) {
 printf("Please enter a port to which The OpenXMLD listens: ");
 scanf("%hd", port_ptr);
}

void xmldclient_print_err(char *err) {
 printf("%s.\n", err);
}

void xmldclient_print_server_err_msg(char *msg) {
 char *p[2];
 p[0]=strchr(msg, ' ');
 p[0]++;
 p[1]=strchr(p[0], '\n');
 *p[1]='\0';
 printf("ERROR %d: ", atoi(p[0]));
 *p[1]='\n';
 p[0]=strchr(p[1], ' ');
 p[0]++;
 p[1]=strchr(p[0], '\n');
 *p[1]='\0';
 printf("%s\n", p[0]);
 *p[1]='\n';
}

void xmldclient_print_record_set(char *rs, struct conn_info *info) {
 char *curr=rs;
 char *curr_col=NULL;
 int curr_col_len=0;
 int level=0;
 int i;
 
 printf(STAR_LINE);
 while (*curr != '\0') {
  if (*curr == info->row_sep) {
   if (curr_col != NULL) {
    printf("%s", curr_col);
    free(curr_col);
    curr_col=NULL;
    curr_col_len=0;
   } 
   printf("\n");
   for (i = 0; i < level; i++) {
    printf("\t");
   }
  }
  else if (*curr == info->col_sep) {
   if (curr_col != NULL) {
    printf("%s", curr_col);
    free(curr_col);
    curr_col=NULL;
    curr_col_len=0;
   } 
   printf(" ");
  }
  else if (*curr == info->down_level) {
   if (curr_col != NULL) {
    printf("%s", curr_col);
    free(curr_col);
    curr_col=NULL;
    curr_col_len=0;
   } 
   level++;
  }
  else if (*curr == info->up_level) {
   if (curr_col != NULL) {
    printf("%s", curr_col);
    free(curr_col);
    curr_col=NULL;
    curr_col_len=0;
   } 
   level--;
  }
  else {
   curr_col=(char *) realloc(curr_col, (curr_col_len + 2) * sizeof(char));
   curr_col[curr_col_len+1]='\0';
   curr_col[curr_col_len]=*curr;
   curr_col_len++;
  }
  curr++;
 }
 if (curr_col != NULL) {
  printf("%s", curr_col);
  free(curr_col);
  curr_col=NULL;
  curr_col_len=0;
 } 
 printf(STAR_LINE);
}

void xmldclient_free_info(struct conn_info *info) {
 cfree(info->col_sep_enc);
 cfree(info->row_sep_enc);
 cfree(info->down_level_enc);
 cfree(info->up_level_enc);
}

char *xmldclient_input_string(char *msg) {
 char curr;
 char *input=NULL;
 int input_curr=0;
 printf("%s: ", msg);
 scanf("%c", &curr);
 while (curr == '\n') {
  scanf("%c", &curr);
 } 
 while (curr != '\n') {
  input=(char *) realloc(input, (input_curr + 2) * sizeof(char));
  input[input_curr + 1]='\0';
  input[input_curr++]=curr;
  scanf("%c", &curr);
 }
 return input;
}

void xmldclient_disconnect(int signum) {
 if (main_fd != 0) {
  protoimpl_write_sequence(main_fd, DISCONNECTION_MESSAGE, 1);
  shutdown(main_fd, 2);
 } 
 printf("\nGoodbye!\n");
 exit(0);
}
