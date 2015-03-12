#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	FILE *fin, *pin; 
	char cmd[255]; 
	double starttime; 

	if(argc!=4 || argc!=5){
		fin = fopen(argv[4], "r"); 
		
		if(fin!=NULL){
			fscanf(fin, "%lf", &starttime); 
			fclose(fin);
			 
			sprintf(cmd, "vlc %s --sout='#rtp{sdp=rtsp://:%s/%s}' --no-loop --no-repeat --play-and-exit --start-time=%.2lf", argv[1], argv[2], argv[3], starttime+1.5); 
		}
		else
			sprintf(cmd, "cvlc %s --sout='#rtp{sdp=rtsp://:%s/%s}' --no-loop --no-repeat --play-and-exit", argv[1], argv[2], argv[3]); 	

		pin = popen(cmd, "r"); 
		pclose(pin); 
	}
	else
		printf("Usage: %s <mediafile> <port> <stream name> [time record file]", argv[0]); 	
		
	return 0; 	
}
