#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>

#define MAXLINE 2048

int main(int argc, char **argv) 
{ 
  int clientfd, port; 
  char* hostName;
  char buff[MAXLINE], recv[MAXLINE];
  
  char* tmpstr1 = "GET ";
  char* tmpstr2 = " HTTP/1.0\r\n\r\n";
  int n = 0;


  hostName = argv[1]; 
  port = atoi(argv[2]); 

  char * cmd = (char *) malloc(1 + strlen(tmpstr1)+ strlen(argv[3]) + strlen(tmpstr2) );
  strcpy(cmd, tmpstr1);
  strcat(cmd, argv[3]);
  strcat(cmd, tmpstr2);
  
  clientfd = open_clientfd(hostName, port); 
  
  strcpy(buff, cmd);
  write(clientfd, buff, strlen(buff)); 
  
  int t = 0;
  while(n = read(clientfd, recv, MAXLINE - 1) > 0){
    fputs(recv, stdout); 
    bzero((char *) &recv, sizeof(recv)); 
  }
   
  close(clientfd); 
  exit(0); 
}

int open_clientfd(char *hostName, int port) 
{ 
  int clientfd; 
  struct hostent *hp; 
  struct sockaddr_in serveraddr; 
 
  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
 	return -1; /* check errno for cause of error */ 
 
 /* Fill in the server's IP address and port */ 
  if ((hp = gethostbyname(hostName)) == NULL) 
 	return -2; /* check h_errno for cause of error */ 
 
  bzero((char *) &serveraddr, sizeof(serveraddr)); 
 
  serveraddr.sin_family = AF_INET; 
 
  bcopy((char *)hp->h_addr, (char *)&serveraddr.sin_addr.s_addr, hp->h_length); 
 
  serveraddr.sin_port = htons(port); 
 
 /* Establish a connection with the server */ 
  if (connect(clientfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) 
 	return -1; 
 
  return clientfd; 
} 

