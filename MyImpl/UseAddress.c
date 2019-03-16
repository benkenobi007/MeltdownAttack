/*
    Sanity Check
    Ensure that no kernel address is directly accessible to the user space
*/
#include<stdio.h>

void main(){
    char * secret_data_addr = (char*)0xfab3b024;
    char secret_data = *secret_data_addr;
    printf("Successfully Accessed\n");
}
