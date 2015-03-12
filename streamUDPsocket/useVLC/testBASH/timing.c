#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

int main(){
	struct timeval tick;  
	double usec; 
	
	gettimeofday(&tick, NULL); 
	
	usec = 1000000 * tick.tv_sec + tick.tv_usec; 
	
	printf("%lf\n", usec);    

	return 0; 	
}
