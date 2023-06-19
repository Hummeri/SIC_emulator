#include<stdio.h>
#include<string.h>

int instruction_find(void);

int main(){
    instruction_find();

}


 int instruction_find(void){

    const char Instruction_set[3][5] = {"NUM1","NUM2","NUM3"} ;

    for(int num=0; Instruction_set[0][num]!='\0'; num++ )
        putchar(Instruction_set[0][num]);

    putchar('\n');


    if(strcmp(&Instruction_set[2][0],"NUM3") == 0)
        printf("!");


    printf("%s",&Instruction_set[1][0]);


    return 0;
}

