#include <stdio.h>
#include <stdlib.h> // for atoi() and malloc

struct bit_shield{
    unsigned int data : 24;
};

int * make_var(int how_many_word);

int main(){

    struct bit_shield shield;
    shield.data = atoi("32");
    printf("data: %d\n", shield.data );
    // overflow test
    //24 bit max value (unsigned!) is 16777216
    shield.data = atoi("16777217");
    printf("saved data: %d original value %d \n", shield.data, atoi("16777217") );




    char input = getchar();
    int number = atoi(&input);
    // printf("input %d \n",number);
    struct bit_shield *shield_ptr= (struct bit_shield *)malloc(sizeof(struct bit_shield)*number );
    putchar('h');

      for(int i =0; i<number;i++){
          shield_ptr[i].data= 24;
    }


    for(int i =0; i<number;i++)
        printf("i: %d value: %d\n",i , shield_ptr[i].data );


}

int * make_var(int how_many_word){ // SIC 머신에서 한 워드가 8비트이다. 레지스터는 모두 3워드, 24비트 이다.
    int *pointer;
    pointer = (int*)malloc(how_many_word*8);
    return pointer;
}

