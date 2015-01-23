#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define MAXLINE 1024

int main(int argc, char *argv[]){
	
	// check usage
	if(argc!=2){
		printf("Usage: %s <port>\n", argv[0]); 
		exit(0); 
	}

	// socket create
	int sockfd; 
	struct sockaddr_in servaddr, clntaddr; 
		// open socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("Socket Error. \n"); 
		exit(1); 	
	}
		// socket bind
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(atoi(argv[1]));
	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
		printf("Socket Bind Error. \n"); 
		exit(1); 	
	} 

	// Get the config file to send later
	FILE *fconf = fopen(".config", "r"); 

	// Wait for client connected
	socklen_t len; 
	char buff[MAXLINE+1]; 
	while(1){
		len = sizeof(clntaddr); 
		memset(buff, 0, sizeof(buff)); 
		recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr*)&clntaddr, &len); 
		printf("Get buff = %s\n", buff); 
		if(!strcmp(buff, "Connection Request. "))
			break;
	}

	// Send Stream
	char mediaName[255]; 
	int offset, count; 	
	while(fscanf(fconf, "%s %d", mediaName, &offset)!=EOF){
		FILE *fin = fopen(mediaName, "r"); 
		printf("Get StreamFile: %s\n", mediaName); 
		if(fin==NULL){
			printf("[%s] does not exist. \n", mediaName); 
			continue;
		}
		count = 0; 
		memset(buff, 0, sizeof(buff)); 
		while(buff[0] = fgetc(fin)){
			if(buff[0]==EOF)
				break;
			count++; 
			if(count<offset)
				continue; 
			sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&clntaddr, len);
			printf("Send buff = %s\n", buff); 
		} 
		fclose(fin); 
	}
	close(sockfd); 
	printf("Socket Close. \n");
	exit(0); 
}
