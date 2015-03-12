#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
	FILE *fstart, *fend, *frec; 
	double timeUse, start, end, rec; 

	if(argc==4){
		fstart = fopen(argv[1], "r"); 
		fend = fopen(argv[2], "r"); 
		frec = fopen(argv[3], "r"); 

		fscanf(fstart, "%lf", &start); 
		fscanf(fend, "%lf", &end); 
		fscanf(frec, "%lf", &rec); 
		
		fclose(fstart); 
		fclose(fend); 
		fclose(frec); 
		
		frec = fopen(argv[3], "w"); 


		timeUse = end - start; 
		timeUse /= 1000000; 

		fprintf(frec, "%lf\n", timeUse+rec); 
	}
	else
		printf("Usage: %s <start file> <end file> <record file>", argv[0]); 
	return 0; 
}
