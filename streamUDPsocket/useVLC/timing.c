#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define MP3FILE "test.mp3"
#define STREAM "test"
#define PORT 8554

int main(int argc, char *argv[]){
	struct timeval start, end;  
	double startTime, timeUse; 
	FILE *fin = NULL, *fout = NULL; 
	char arg1[255] = {}, arg2[255] = {}; 
	if(argc!=2 && argc!=3)
		printf("Usage: %s <record file> [start time record file]", argv[0]); 


	/* prepare of all files */
	if(argc==3)								// start time record file
		fin = fopen(argv[2], "r"); 		 
	fout = fopen(argv[1], "w"); 			// record file

	/* prepare of command variables */
	if(argc==3)
		fscanf(fin, "%lf", &startTime); 

	gettimeofday(&start, NULL); 

	sprintf(arg1, "--sout='#rtp{sdp=rtsp://:%d/%s}'", PORT, STREAM); 
	if(argc==3)
		sprintf(arg2, "--start-time=%.2lf", startTime); 
	execlp("cvlc", "cvlc", "-vvv", MP3FILE, "--sout-standard-access=udp", "--sout-standard-mux=rtp", "--sout='#transcode{vcodec=h264, acodec=mpga, ab=128, channels=2, samplerate=44100}'", arg1, arg2, NULL); 
	printf("cvlc -vvv %s %s %s\n", MP3FILE, arg1, arg2); 


	fork(); 
	gettimeofday(&end, NULL);
	timeUse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec; 
	timeUse /= 1000000;
	
	printf("%lf\n", timeUse);    
	fprintf(fout, "%lf\n", timeUse); 
	if(fin!=NULL)
		fclose(fin); 
	fclose(fout); 

	return 0; 	
}
