
#include<stdio.h>
#include<stdlib.h> //used for fopen
#include<string.h> //used for strcmp()
#include<ctype.h> //used for isupper()
#include<stdbool.h> //for bool flags

int instruction_find(void);
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

    while(1){
        ch = getc(fp);
        printf("got it \n");
        if(ch == EOF){
            instruc_char_index=0;
            while(instruction_buffer[instruc_char_index] !='\0'){
                putchar(instruction_buffer[instruc_char_index]);
                instruc_char_index++;
            }
            break;
        }



        if( ch != ' '){
            instruction_buffer[instruc_char_index]=ch;
            instruc_char_index++;
            printf("%d",instruc_char_index);
        }
        else {
            instruction_buffer[instruc_char_index]='\0';
            instruc_char_index=0;
            while(instruction_buffer[instruc_char_index] !='\0'){
                putchar(instruction_buffer[instruc_char_index]);
                instruc_char_index++;
            }
            instruc_char_index=0;
            //printf("%s",&instruction_buffer[0]);
            printf("\n");
        }

    }

    printf("\n");
    /*
    while( (ch=getc(fp) ) != EOF )
    {
        putc(ch,stdout);


        if(is_not_char){
            if(ch==' '){
                count++;
                continue;
            }

        }

        else if(isupper(ch)){
            instruction_buffer[instruc_char_index]=ch;
            instruc_char_index++;
        }
        else if(ch==' '){
            instruction_buffer[instruc_char_index]='\0';
            printf("%s",instruction_buffer[1]);
            is_not_char=true;
            instruc_char_index=0;
        }
        else
            printf("\nSpace!_2\n");



        count++;
    }
    */
    fclose(fp); //얘를 않쓰면 어떻게 되지?

    printf("%s에는 %ld개의 문자, %ld개의 대문자가 들어 있습니다.\n",argv[1],count,Upper);

    return 0;
}

 int instruction_find(void){

    const char Instruction_set[2][5] = {"ADD","SUB"} ;



    if(strcmp(&Instruction_set[0][0],"ADD") == 0)
        printf("!");





    return 0;
}

