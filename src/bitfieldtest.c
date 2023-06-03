#include <stdio.h>
#include <stdlib.h> // for atoi() and malloc
#include<stdbool.h>

struct bit24{
    unsigned int data : 24;
};
struct variable{
    char name[20];
    struct bit24 *ptr; // for char variables, the 8bit memory size is "emulated"
    bool is_array;
    int array_max;
    bool data_type;
};
struct bit24 * make_var(int how_many_word);


int main(){

    struct variable varList[10];
    char input = getchar();

    int number = atoi(&input);
    for(int varIndex=0; varIndex < 10; varIndex++){
        varList[varIndex].ptr= make_var(5*sizeof(struct bit24));
        for(int i=0;i<5;i++){
            (varList[varIndex].ptr+i*sizeof(struct bit24) )->data = 24;
        }
    }

        for(int varIndex=0; varIndex < 10; varIndex++){
        varList[varIndex].ptr= make_var(5*sizeof(struct bit24));
        for(int i=0;i<5;i++){
            (varList[varIndex].ptr+i*sizeof(struct bit24) )->data = 24;
        }
    }


}

struct bit24 * make_var(int how_many_word){ // how_many_word 24비트 저장소 몇개?
    struct bit24 *pointer;
    pointer = malloc(how_many_word* sizeof(struct bit24));
    return pointer;
}



