 #include<stdio.h>
#include<stdlib.h> //used for fopen and mallac() and free()
#include<string.h> //used for strcmp() and strcmp()
#include<ctype.h> //used for isupper()
#include<stdbool.h> //for bool flags

int instruction_find(char *check);
//int program_initiate( char*program_name1,);

struct Word{
    char words[20]; //insruction buffer gets flushed here
    int type; // 0이면 value, 1~25 사이의 값은 명령어, 26은 라벨, 27 제목 값이다.
    int line; //몇 번째 줄에 값이 있는지 나타낸다.
    };
int assembler(struct Word *keywords,int index_max); //make sure that functions using struct is declared after the declaration of struct

bool num_check(char *address);

char * make_var(int how_many_word);

int main(int argc,char *argv[])
{
    int ch; //왜 ch은 int형인가?
    FILE *fp;
    unsigned long count =0;
    unsigned long Upper =0;
    char program_name[256];

    //프로그램이름+입력된 파일이름 말고도 파일이름이 하나 추가될때 이런 에러가 나타난다.
    if(argc!=2) {
        printf("usage: %s filename\n",argv[0] );
        exit(EXIT_FAILURE); //얘는 stdlib에서 정의된다
    }

    if( (fp = fopen(argv[1],"r")) == NULL ){
        printf("failed to open fail %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }


    struct Word Program[50];
    //struct Word temp;
    int Word_line=0;
    int Word_count=0;
    int word_struct_index=0;
    int instruc_char_index=0;
    char instruction_buffer[10];
    bool not_empty_space=0;


    while( (ch = getc(fp))!=EOF ){


        putchar(ch);
        if( isalnum(ch) ){ //isalnum 은 스페이스나 \n을 만나면 멈춘다
            instruction_buffer[instruc_char_index]=ch;
                instruc_char_index++;
                not_empty_space=1;
        }
        else if(ispunct(ch) ){ // ' 같은 문자들을 인식하게 한다.
            instruction_buffer[instruc_char_index]=ch;
                instruc_char_index++;
                not_empty_space=1;
                continue;
        }
        else if(ch==' ' || ch=='\t' && not_empty_space ){ //ch가 스페이스 일때,
            instruction_buffer[instruc_char_index]='\0'; //버퍼속의 문자를 문자열로 만듬
            strcpy(Program[word_struct_index].words , &instruction_buffer[0]);

            Program[word_struct_index].type = instruction_find(&instruction_buffer[0]);
            Program[word_struct_index].line = Word_line;
            word_struct_index ++;
            Word_count++;
            instruc_char_index=0; // instruction_buffer의 내용물을 치우지 않고 그냥 인덱스를 0으로 만든다, strcmp 같은 함수들이 '\0'을 무시하고
            not_empty_space=0;
        }
        //ch가 개행 문자 일때 struct word에 저장한 텍스트 버퍼가 명령어인지, 아닌지 검사후 struct word.type에 검사값 저장.
        else if(ch == '\n' && not_empty_space){ // && not_empty_space을 빈 여백의 코드를 그냥 걸러낸다. 주석 달기 기능과, 사용자의 입장에서 어떤 줄에서 에러가 발생 했는지 나타내려면 저걸 없애고, 키워드을 정리하는 프로그램이 얘를 처리해야 한다.
            instruction_buffer[instruc_char_index]='\0'; //버퍼속의 문자를 문자열로 만듬
            strcpy(Program[word_struct_index].words , &instruction_buffer[0]);
            Program[word_struct_index].type = instruction_find(&instruction_buffer[0]);
            Program[word_struct_index].line = Word_line;
            word_struct_index ++;
            Word_line ++;
            Word_count++;
            instruc_char_index=0;
            not_empty_space=0;
        }


    }

for (int index=0; index <Word_count; index++){
    printf("words:%s: type: %d, line: %d\n",Program[index].words, Program[index].type , Program[index].line);
}

    fclose(fp); //얘를 않쓰면 어떻게 되지?

    putchar('\n');
    assembler(&Program[0],Word_count);

    return 0;
} //end of main

int assembler(struct Word *keywords,int index_max){
    //title and start and memory start address check
    if(keywords[0].type == 0)
        if(strcmp(keywords[1].words,"START")==0 && keywords[0].line == keywords[1].line)
            if(keywords[1].line == keywords[2].line  && num_check( &keywords[2].words[0]) ) //needs special num check that numerates through string.
                printf("valid start\n");
    else
        printf("not valid start\n");

    int index =3;

    while( index < index_max) {
        if(keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line && keywords[index+2].line == keywords[index+3].line){ // 같은 줄에 키워드가 네개 에러 발생!
            printf("instruction format error at line: %d more than three keywords\n", keywords[index].line);
            break;
        }
        else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line){ // 다음 명령어 세개가 같은 줄에 있다.
            printf("instruction line with label\n");
            // 변수 선언문도 라벨이랑 같은 3개의 문자 형식을 가지고 있으나, word.type에서 다른 형식을 지니고 있는 것이니, 여기서 명령어 타입 검사해서 변수 선언 키워드가 있으면 여기 있는 줄이 변수가 있는 줄로 간주한다.
            //변수들의 타입은 25번이면 END, 그후 30번까지가 변수 영역에 쓰이는 키워드이다.

            if( keywords[index].type == 0 && keywords[index+1].type > 25 && keywords[index+1].type <= 30 && num_check(&keywords[index+2].words[0])){
                printf("properly declared variable\n");
                printf("instruction line found at %d\n", keywords[index].line+1); // 컴퓨터는 첫번째 줄을 0번째 줄이라고 하기 때문에 사람이 일기 편하게 하려면 1을 더해야 한다.
                break;
            }
            index += 3;
        }
        else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line +1  == keywords[index+2].line){ // 다음 두 명령어가 같은 줄에 있고, 다음 명령어는 다음 줄에 있다.
            printf("instruction line without label\n");
            index +=2;
        }


    }

    char* ptr;
    ptr = make_var(1);
    //*ptr = 0;
    *ptr = 0;
    //printf("%d");

    printf("%d\n",*ptr);
    *ptr = 255;
    printf("%d\n",*ptr);
    free(ptr);



    return 0;
}

char * make_var(int how_many_word){
    char *pointer;
    pointer = (char*)malloc(how_many_word*8);
    return pointer;
}

bool num_check(char *address){
    for(int i=0; address[i] != '\0'; i++)
        if(isdigit( address[i])==0 ) // isdigit 값이 0이면 해당 값이 숫자가 아니라는 소리이다
            return 0;

    return 1;
}

 int instruction_find(char *check){

    const char Instruction_set[30][5] = {"ADD","SUB","MUL","DIV","AND","OR","LDA","LDCH","LDL","LDX","STA","STCH","STL","STX","COMP","TIX","J","JEQ","JGT","JLT","JSUB","RSUB","TD","RD","WD","END","BYTE","WORD"/* WORD 28*/,"RESB","RESW"} ;

    int for_return=0;


    for(int x=0; x<30; x++){
        if(strcmp(check,&Instruction_set[x][0]) == 0){
            //printf(" %s detected!\n", &Instruction_set[x][0]) ;
            for_return = x+1;
            break;
        }
    }

    return for_return;
}
