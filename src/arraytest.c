 #include<stdio.h>
#include<stdlib.h>
int * make_var(int how_many_word);

int main(){
    int *ptr=make_var(2);
    *ptr=1;
    *(ptr+24)=2;
    printf("first value: %d second value: %d",*ptr,*(ptr+24));
}

 int * make_var(int how_many_word){ // SIC 머신에서 한 워드가 8비트이다. 레지스터는 모두 3워드, 24비트 이다.
    int *pointer;
    pointer = (int*)malloc(how_many_word*8);
    return pointer;
}
