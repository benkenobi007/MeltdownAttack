/*
Implementing exception handling in C.
Catch the occurence of SIGSEGV and roll back
*/

#include<stdio.h>
#include<signal.h>
#include<setjmp.h>

static sigjmp_buf jbuf;     //Buffer holding the checkpoints

//Exception handler, jumps to the earlier checkpoibnt
static void catch_segv(){
    printf("ERROR : Sigsegv. Rolling back ... \n");
    siglongjmp(jbuf, 1);
}

void main(){
    unsigned int secret_address = 0xfab3b024;   //secret data address

    //Register catch_segv() exception handler for sigsegv
    signal(SIGSEGV, catch_segv);

    //try to set checkpoint state
    if(sigsetjmp(jbuf, 1)==0){
        char data = *(char*)secret_address;     //raises sigsegv
        printf("Accessed the data %c at address %u", data, secret_address);         //not executed
    }
    else{
        //if checkpoint was not set
        printf("Memory Access violation\n");
    }

    printf("Continue execution\n");
}
