#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
 int fd=socket(PF_INET, SOCK_STREAM, 0);
 
 if (fd == -1) {
  printf("Error creating socket");
  return 0;
 }
 
 struct sockaddr_in addr;
 addr.sin_family=AF_INET;
 addr.sin_port=htons(6665);
 addr.sin_addr.s_addr=htonl(INADDR_ANY);
 int status=connect(fd, (struct sockaddr *) &addr, sizeof(addr));
 
 if (status == -1) {
  perror("connect");
  return 0;
 }
 
 char query[150];
 strcpy(query, "select name from \"my_file.xml\" where !:(tagname)=\"c\"");
 status=write(fd, (void *) query, 150);
 
 if (status == -1) {
  perror("send");
  return 0;
 }
 
 char resp[300];
 resp[299]='\0';
 status=read(fd, (void *) resp, 300);
 
 if (status == -1) {
  perror("recv");
  return 0;
 }
 
 printf("%s\n", resp);
 return 0;
}
