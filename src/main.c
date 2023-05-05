
#include<stdio.h>
#include<stdlib.h> //used for fopen
#include<string.h> //used for strcmp()
#include<ctype.h> //used for isupper()
#include<stdbool.h> //for bool flags

int instruction_find(char *check);
//int program_initiate( char*program_name1,);

int main(int argc,char *argv[])
{
    int ch; //왜 ch은 int형인가?
    FILE *fp;
    unsigned long count =0;
    unsigned long Upper =0;
    char program_name[256];
    char instruction_buffer[10];

    if(argc!=2) //프로그램이름+입력된 파일이름 말고도 파일이름이 하나 추가될때 이런 에러가 나타난다.
    {
        printf("usage: %s filename\n",argv[0] );
        exit(EXIT_FAILURE); //얘는 stdlib에서 정의된다
    }

    if( (fp = fopen(argv[1],"r")) == NULL )
    {
        printf("failed to open fail %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    bool started=1;
    bool is_not_char=0;
    int instruc_char_index=0;

    while( (ch = getc(fp))!=EOF ){

        printf("@");

        if( ch != ' '){
            instruction_buffer[instruc_char_index]=ch;
            instruc_char_index++;
        }
        else {
            instruction_buffer[instruc_char_index]='\0'; //is this code neccessary?
            instruction_find(&instruction_buffer[0]);
            instruc_char_index=0;

        }

    }
    instruction_buffer[instruc_char_index]='\0';
    instruction_find(&instruction_buffer[0]); //flushes last buffer



    fclose(fp); //얘를 않쓰면 어떻게 되지?




    return 0;
}

 int instruction_find(char *check){

    const char Instruction_set[25][5] = {"ADD","SUB","MUL","DIV","AND","OR","LDA","LDCH","LDL","LDX","STA","STCH","STL","STX","COMP","TIX","J","JEQ","JGT","JLT","JSUB","RSUB","TD","RD","WD"} ;


    bool yes =0;

    for(int x=0; x<25; x++){
        if(strcmp(check,&Instruction_set[x][0]) == 0){
            printf(" %s detected!\n", &Instruction_set[x][0]) ;
            yes =1;
            break;
        }
    }
    if (yes==0)
        printf("nope\n");
    return 0;
}

