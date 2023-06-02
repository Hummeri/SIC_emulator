#include <stdio.h>
#include <stdlib.h> // for atoi()

struct bit_shield{
    unsigned int data : 24;
};

int main(){
    struct bit_shield shield;
    shield.data = atoi("32");
    printf("data: %d\n", shield.data );
    // overflow test
    //24 bit max value (unsigned!) is 16777216
    shield.data = atoi("16777217");
    printf("saved data: %d original value %d \n", shield.data, atoi("16777217") );

}

