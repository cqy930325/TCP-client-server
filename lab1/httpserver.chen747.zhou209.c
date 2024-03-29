

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#define VERSION 23
#define MAXLINE 2048
#define NOTFOUND 404
#define FORBIDDEN 403
#define ERROR 11
#define NORMAL 12
void command(int type,int connfd)
{
	switch (type) {
		case ERROR:
			(void)write(connfd, "HTTP/1.1 403 Forbidden\r\n\r\n",27);
			break;
		case FORBIDDEN:
			(void)write(connfd, "HTTP/1.1 403 Forbidden\r\n\r\n",27);
			break;
		case NOTFOUND:
			(void)write(connfd, "HTTP/1.1 404 NOT FOUND\r\n\r\n",27);
			break;	
		case NORMAL:
			(void)write(connfd, "HTTP/1.1 200 OK\r\n\r\n",19);
			break;
	}
	if(type == ERROR || type == FORBIDDEN || type == NOTFOUND)
		exit(3);
}
int open_listenfd(int port){
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
		(const void *)&optval , sizeof(int)) < 0)
		return -1; 
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serveraddr.sin_port = htons((unsigned short)port);
	if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
 		return -1; 

 	/* Make it a listening socket ready to accept 
 	connection requests */ 
 	if (listen(listenfd, 64) < 0) 
 		return -1; 
 	return listenfd; 
}

void transfer(int connfd)
{
	long i,n,len;
	int file_id;
	char rbuf[MAXLINE];
	char wbuf[MAXLINE];
	n = read(connfd,rbuf,MAXLINE);
	if(n<=0){
		command(FORBIDDEN,connfd);
	}
	else{
		rbuf[n] = 0;
	}
	for(i = 0;i<n;i++){
		if(rbuf[i] == '\r' || rbuf[i] == '\n')
			rbuf[i] = '+';
	}
	if(strncmp(rbuf,"GET ",4) != 0){
		command(ERROR,connfd);
	}
	for(i = 0;i<n;i++){
		if(rbuf[i] == ' ')
			rbuf[i] = 0;
	}
	if(( file_id = open(&rbuf[4],O_RDONLY)) == -1) {  /* open the file for reading */
		if(errno == EACCES)
			command(FORBIDDEN,connfd);
		else
			command(NOTFOUND,connfd);
	}
	command(NORMAL,connfd);
	while((n = read(file_id, wbuf,MAXLINE)) > 0){
		write(connfd,wbuf,n);
	}
	close(connfd);
	exit(1);
}


int main(int argc, char **argv) {
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;

	port = atoi(argv[1]);
	listenfd = open_listenfd(port);

	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
		hp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,
			sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		transfer(connfd);
		close(connfd);
	}
}
