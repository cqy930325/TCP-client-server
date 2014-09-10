#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 32
void Die(char *mess) { perror(mess); exit(1);}
int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_server;
	char buffer[BUFFERSIZE];
	int received = 0;

	if(argc != 4) {
		fprintf(stderr, "USAGE: httpclient.chen747.zhou209 <server address> <port> <Filepath>\n");
		exit(1);
	}

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		Die("Failed to create socket");
	}

	
}