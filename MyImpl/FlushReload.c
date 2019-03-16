#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<emmintrin.h>
#include<time.h>

#define DELTA 1024

uint8_t array[256*4096];

void flushSideChannel(){
	
	int i;
	for(i=0;i<256;i++) array[i*4096 + DELTA]=1;
	
	for(i=0;i<256;i++) _mm_clflush(&array[i*4096 + DELTA]);
}

void victim(){
	int secret = 95;
	array[secret * 4096 + DELTA] = 1;
}

void attack(){
	uint64_t diff;
	volatile uint8_t *addr;
	double min = 100000000;
	struct timespec start, end;
	int i, temp, k;
	for(i=0;i<256;i++)
	{
		addr=&array[i*4096+DELTA];
		clock_gettime(CLOCK_MONOTONIC, &start);
		temp = *addr;
		clock_gettime(CLOCK_MONOTONIC, &end);
		diff = end.tv_nsec - start.tv_nsec;
		if(min >= diff)
		{
			min=diff;
			k = i;
		}	
	}
	
	printf("Secret key = %d\n", k);		
}

int main(){
	flushSideChannel();
	victim();
	attack();
	return 0;
}
