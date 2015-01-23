#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]){
	int i, j; 
	FILE *fout; 
	srand(time(NULL)); 
	
	for(i=1;i<argc;i++){
		fout = fopen(argv[i], "w"); 
		j = rand()%(1024*1024*1024)+1; 
		while(j--)
			fprintf(fout, "%c", rand()%256); 
		fclose(fout); 
	}
	return 0; 	
}
