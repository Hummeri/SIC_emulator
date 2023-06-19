#include<stdio.h>
#include<stdlib.h>

void getout(void);

int main(){
    printf("test1\n");
    getout();
    printf("did not exit\n");
    return 0;
}

void getout(void){
    exit(0);
}
