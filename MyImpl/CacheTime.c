#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<emmintrin.h>

#define BILLION 1E9
uint8_t array[10*4096];

int main(){
    // uint64_t diff;
    float diff;
    struct timespec start, end;
    int i, temp;
    volatile uint8_t *addr;
    for(i=0;i<10;i++) array[i*4096] = 1;

    for(i=0;i<10;i++) _mm_clflush(&array[i*4096]);

    array[5*4096] = 10;
    // array[8*4096] = 20;
    

    for(i=0;i<10;i++){
        addr = &array[i*4096];
        clock_gettime(CLOCK_MONOTONIC, &start);
        // temp = array[i*4096];
        temp = *addr;
        clock_gettime(CLOCK_MONOTONIC, &end);
//        diff = BILLION*(end.tv_sec - start.tv_sec) + (end.tv_nsec-start.tv_nsec);
		diff = (end.tv_nsec-start.tv_nsec);
        printf("array[%d*4096] time = %f\n",i, diff);
    }
    return 0;
}
