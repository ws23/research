#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>	

#define MEDIAFILE "test.mp3"
#define STREAM "test"
#define PORT 8554
#define STARTFILE "start.txt"


void *calcTime();	//	Calcutate the timing 
void *execCVLC();	//	Execute the VLC
void *controlQ();	//	Control which time need to exit VLC

int state;	// 0.begin	1.execVLC	2.exitVLC 3.interruptVLC
double startTime; 

pthread_t tid1, tid2, tid3; 

int main(){
	FILE *fin; 
	
	state = 0; 
	startTime = 0; 
	fin = fopen(STARTFILE, "r"); 		 
	fscanf(fin, "%lf", &startTime); 

	pthread_create(&tid2, NULL, &execCVLC, NULL); 
	pthread_create(&tid3, NULL, &controlQ, NULL); 
	pthread_create(&tid1, NULL, &calcTime, NULL); 

	pthread_join(tid2, NULL); 
	pthread_join(tid3, NULL); 
	pthread_join(tid1, NULL); 

	return 0; 	
}

void *calcTime(){
	struct timeval start, end; 
	double timeUse; 
	FILE *fd, *fout = fopen(STARTFILE, "w"); 
	int i, pid; 

	gettimeofday(&start, NULL); 
	while(state<2){
		printf("calTime: %d\n", state); 
		sleep(1); 
	} 
	gettimeofday(&end, NULL); 
	
	timeUse = 1000000 * ( end.tv_sec-start.tv_sec ) + end.tv_usec-start.tv_usec; 
	timeUse /= 1000000; 

	if(state==3){
		printf("%lf\n", timeUse+startTime); 
		fprintf(fout, "%lf\n", timeUse+startTime); 
	}
	else if(state==2){
		printf("0\n"); 
		fprintf(fout, "0\n"); 
	}
	fd = popen("ps | grep vlc", "r"); 
	fscanf(fd, "%d", &pid); 
	pclose(fd); 
	fclose(fout); 
	kill(pid, SIGKILL); 
	exit(0); 
}

void *execCVLC(){
	char cmd[255], cmd2[255]; 
	FILE *fd; 
	int err; 
	state = 1; 
	sprintf(cmd, "cvlc %s --no-loop --no-repeat --play-and-exit --sout='#rtp{sdp=rtsp://:%d/%s}' --start-time=%.2lf", MEDIAFILE, PORT, STREAM, startTime); 

	err = system(cmd); 	
	if(err==0)
		state = 2; 
	else
		state = 4; 
	pthread_exit(NULL); 
}


void *controlQ(){
	char c; 
	while(state==0){
		printf("controlQ: %d\n", state); 	
	}
	printf("Enter 'q' to Exit. \n");  
	do{
		c = getchar(); 
	}while(c!='q'); 
	state = 3; 
	pthread_exit(NULL); 
}
