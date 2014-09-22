#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>

#define MAXLINE 2048
#define NOTFOUND 404
#define FORBIDDEN 403
#define ERROR 11
#define NORMAL 12

int main(int argc, char **argv) {
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;

	port = atoi(argv[1]);
	listenfd = open_listenfd(port);

	while (1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
		hp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,
			sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		//printf("FD %d connected to %s (%s:%s)\n",
		//	connfd, hp->h_name, haddrp, ntohs(clientaddr.sin_port));
		transfer(connfd);
		close(connfd);
	}
}
int open_listenfd(int port){
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) 
 	return -1; 
 	/* Make it a listening socket ready to accept 
 	connection requests */ 
 	if (listen(listenfd, LISTENQ) < 0) 
 	return -1; 
 	return listenfd; 
}

void transfer(int connfd)
{
	long i,n,len;
	int file_fd;
	char rbuf[MAXLINE];
	char wbuf[MAXLINE];
	n = read(connfd,rbuf,MAXLINE);
	if(buf<=0){
		command(FORBIDDEN,connfd);
	}
	else{
		rbuf[n] = 0;
	}
	for(i = 0;i<n;i++){
		if(rbuf[i] == "\r" || rbuf[i] == "\n")
			rbuf[i] = "+";
	}
	if(strncmp(rbuf,"GET ",4) != 0){
		command(ERROR,connfd);
	}
	for(i = 0;i<n;i++){
		if(rbuf[i] == " ")
			rbuf[i] = 0;
	}
	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) {  /* open the file for reading */
		command(NOTFOUND,connfd);
	}
	command(NORMAL,connfd);
	while((n = read(file_id, wbuf,MAXLINE)) > 0){
		write(connfd,wbuf,n);
	}
	close(connfd);
	exit(1);
}
void command(int type,int connfd)
{
	switch (type) {
		case ERROR:
		case FORBIDDEN:
		case NOTFOUND:
		case NORMAL:
	}
}
