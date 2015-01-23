#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAXLINE 1024

int main(int argc, char *argv[]){

	// check usage
	if(argc!=3){
		printf("Usage: %s <IP address> <port>\n", argv[0]); 
		exit(0);
	}

	// socket create
	struct sockaddr_in servaddr; 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(atoi(argv[2])); 
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr)<=0){
		printf("inet_pton Error For %s\n", argv[1])	; 
		exit(1); 	
	}

	// Send Streaming Connect Request
	int sockfd; 
	socklen_t len; 
	char buff[MAXLINE+1]; 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	len = sizeof(servaddr); 
	memset(buff, 0, sizeof(buff)); 
	strcpy(buff, "Connection Request. "); 
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, len); 
	printf("Send buff = %s\n", buff); 

	// Wait for Server's streaming & Display them
	while(1){
		memset(buff, 0, sizeof(buff));
		recvfrom(sockfd, buff, MAXLINE, 0, NULL, NULL); 
		printf("Get buff = %s\n", buff); 
	}
	exit(0); 
}

