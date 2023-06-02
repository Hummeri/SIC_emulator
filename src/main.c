#include<stdio.h>
#include<stdlib.h> //used for fopen and mallac() and free()
#include<string.h> //used for strcmp() and strcmp()
#include<ctype.h> //used for isupper()
#include<stdbool.h> //for bool flags

#define WORD_MAX_LENGTH 20 //used for max character length of title and variable names.
#define MAX_VARIABLE_COUNT 20 //최대 변수 개수
#define MAX_LABEL_COUNT 20 //라벨 최대 개수
#define MAX_PROGRAM_INSTRUCTIONS 100 // 명령어 최대 개수

int instruction_find(char *check);
//int program_initiate( char*program_name1,);

struct Word{
    char words[WORD_MAX_LENGTH]; //insruction buffer gets flushed here
    int type; // 0이면 value, 1~25 사이의 값은 명령어, 26은 라벨, 27 제목 값이다.
    int line; //몇 번째 줄에 값이 있는지 나타낸다.
    };
int assembler(struct Word *keywords,int index_max); //make sure that functions using struct is declared after the declaration of struct

struct variable{
    char name[WORD_MAX_LENGTH];
    int *ptr;
    bool is_array;
    int array_max;
    bool data_type;
};

struct executable{
    int instruction;
    int variable_index;
    int label_index; // if label index is 0, there is no index.
};

struct label{
    char name[WORD_MAX_LENGTH];
    int to_here;
};

struct bit_shield_24{
    unsigned int data : 24;
};

struct bit_shield_8{
    unsigned int data : 24;
};

bool num_check(char *address);
short data_check(char *address2);

int * make_var(int how_many_word);

void MathCalculate(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void LoadFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void StoreFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void CompareFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void IOFunction(int instruction,int *Register_A);

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
    //printf("\n %d",Word_count);

    fclose(fp); //얘를 않쓰면 어떻게 되지?

    putchar('\n');
    assembler(&Program[0],Word_count);

    return 0;
} //end of main


int assembler(struct Word *keywords,int index_max){
    //title and start and memory start address check
    char title[WORD_MAX_LENGTH];

    if(keywords[0].type == 0)
        if(strcmp(keywords[1].words,"START")==0 && keywords[0].line == keywords[1].line)
            if(keywords[1].line == keywords[2].line  && num_check( &keywords[2].words[0]) ){
                printf("valid start\n");
                strcpy(&title[0],keywords[0].words);
                //printf("%c\n",title[0]);
            }
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

            if( keywords[index].type == 0 && keywords[index+1].type > 25 && keywords[index+1].type <= 30 && data_check(&keywords[index+2].words[0])){
                printf("properly declared variable\n");
                printf("instruction line found at %d\n", keywords[index].line/*+1*/); // 컴퓨터는 첫번째 줄을 0번째 줄이라고 하기 때문에 사람이 읽기 편하게 하려면 1을 더해야 한다.
                break;
            }
            index += 3;
        }
        else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line +1  == keywords[index+2].line){ // 다음 두 명령어가 같은 줄에 있고, 다음 명령어는 다음 줄에 있다.
            printf("instruction line without label\n");
            index +=2;
        }
    }

    int var_index_start = index;
    struct variable variable_list[MAX_VARIABLE_COUNT];
    int var_index=0;
    printf("index max %d\n",index_max);
    while( strcmp(keywords[index].words,"END") != 0 && index < index_max){
        //printf("END not found at index %d\n",index);
        index++;
    }
    if(strcmp(keywords[index].words,"END") != 0 ){
        printf("ERROR no END keyword found!\n");
    }

    int var_index_max = index; // 변수 배열을 생성하는 코드
    index = var_index_start;
    //printf("words:%s: type: %d, line: %d\n",keywords[index].words, keywords[index].type , keywords[index].line);

    while(index<var_index_max){
        if( keywords[index].line == keywords[index+1].line && keywords[index+1].line== keywords[index+2].line
&& keywords[index].type == 0 && keywords[index+1].type > 25 && keywords[index+1].type <= 30 && data_check(&keywords[index+2].words[0])){
            printf("valid variable ");
            strcpy( variable_list[var_index].name, keywords[index].words);

            if( keywords[index+1].type == 27) { //변수 타입 BYTE 얘는 CHAR1 BYTE C'Z' 같은 형식을 사용할 수 있으니 저 c와 '를 검사하는 걸 넣어야한다.
                variable_list[var_index].ptr = make_var(1); //1 바이트, 8비트의 크기를 가진 메모리 주소를 리턴한다.

                if( data_check( &keywords[index+2].words[0] ) == 2){ // 단일 문자 저장
                    *variable_list[var_index].ptr =keywords[index+2].words[2];

                    variable_list[var_index].is_array=0;
                    variable_list[var_index].data_type = 1;

                    printf("BYTE: name:%s value: %c is_array: %d\n",variable_list[var_index].name ,*variable_list[var_index].ptr,variable_list[var_index].is_array);
                }
            }
            else if(keywords[index+1].type == 28){  //변수 타입 WORD
                variable_list[var_index].data_type = 0;

                int comma_count =0;
                for(int i =0; keywords[index+2].words[i] != '\0' && i < WORD_MAX_LENGTH; i++){
                    if(keywords[index+2].words[i] == ',')
                        comma_count++;
                }
                if(comma_count==0){
                    variable_list[var_index].ptr = make_var(3);
                    *variable_list[var_index].ptr = atoi(&keywords[index+2].words[0]); //the atoi function requires const str* type, but somehow still works
                    variable_list[var_index].is_array=0;
                }
                else{
                    variable_list[var_index].ptr = make_var( 3* (comma_count+1) );
                    char buffer[WORD_MAX_LENGTH];
                    int pointer_offset =0;
                    int buffer_index=0;

                    for(int i=0; i < WORD_MAX_LENGTH; i++){
                        if(keywords[index+2].words[i] == ','){
                            buffer[buffer_index]='\0';
                            printf("that string is %d as a number\n ",atoi(&buffer[0]) );
                            *(variable_list[var_index].ptr+ pointer_offset*24 )=atoi(&buffer[0]); //check atoi return type!
                            buffer_index=0;
                            pointer_offset++;
                        }
                        else{
                            buffer[buffer_index]=keywords[index+2].words[i];
                            buffer_index++;
                        }

                        if(keywords[index+2].words[i] == '\0' /*|| pointer_offset == comma_count*/){
                            buffer[buffer_index]='\0';
                            printf("buffer last : %s\n",&buffer[0]);

                            printf("that string is %d as a number\n ",atoi(&buffer[0]) );
                            struct bit_shield_24 shield_temp;
                            shield_temp.data = atoi(&buffer[0]);
                            *(variable_list[var_index].ptr+ pointer_offset*24 )= shield_temp.data;
                            printf("value here: %d\n",*(variable_list[var_index].ptr+pointer_offset*24));

                            variable_list[var_index].is_array=1;
                            variable_list[var_index].array_max = comma_count +1;
                            break;
                        }

                    }


                }

                printf("WORD, name:%s value: %d is_array:%d\n",variable_list[var_index].name ,*variable_list[var_index].ptr,variable_list[var_index].is_array);
            }
            else if(keywords[index+1].type == 29) { //RESB
                int temp = atoi(&keywords[index+2].words[0]);
                variable_list[var_index].ptr = make_var( temp );
                variable_list[var_index].data_type = 1;

                if(temp > 1){
                    variable_list[var_index].is_array=1;
                    variable_list[var_index].array_max=temp;
                }
                else
                    variable_list[var_index].is_array=0;

                printf("RESB, name:%s is_array:%d\n", variable_list[var_index].name,variable_list[var_index].is_array);
                }
            else if(keywords[index+1].type == 30){ // RESW
                int temp = atoi(&keywords[index+2].words[0])*3;
                variable_list[var_index].ptr = make_var( temp );
                variable_list[var_index].data_type = 0;
                if(temp >1){
                    variable_list[var_index].is_array=1;
                    variable_list[var_index].array_max=temp;
                }
                else
                    variable_list[var_index].is_array=0;
                printf("RESW: name:%s is_array:%d \n", variable_list[var_index].name,variable_list[var_index].is_array);
                }
            }
        index +=3;
        var_index++;
        }
    int variable_total_count = var_index;
    printf("variable_total_count: %d \n=====================\n", variable_total_count);


    index =3;

        // 명령어 집합하기
    struct label label_list[MAX_LABEL_COUNT];
    struct executable executable_list[MAX_PROGRAM_INSTRUCTIONS];
    int executable_index=0;
    int label_index=0;
    //printf("%d\n",index_max);
        while( index < var_index_start) {
            if( keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line){ // 다음 명령어 세개가 같은 줄에 있다.
                printf("%d: instruction line with label\n",keywords[index].line);
                strcpy(&label_list[label_index].name[0], keywords[index].words);  //라벨 저장
                label_list[label_index].to_here = executable_index;
                label_index++;
                executable_list[executable_index].label_index= label_index;// struct executable에서 label_index이 0이면 라벨이 없다고 판단한다. 따라서 1을 더한 값을 여기에다가 저장한다. 나중에 라벨을 찾을 때는 저 인덱스 값을 1 빼준 다음 액세스 해야한다.
                executable_list[executable_index].instruction = keywords[index+1].type;

                bool success_flag=0;
                for(int i=0; i<variable_total_count; i++){
                    if( strcmp(&variable_list[i].name[0], keywords[index+2].words) == 0 ){ // 명령어 줄에 있는 변수 이름이 선언된었는지 확인한다.
                        executable_list[executable_index].variable_index = i;
                        variable_list[i].is_array=0;
                        success_flag =1;
                        break;
                    }
                }

                if(success_flag ==0){ // if a normal name was not detected, check if it's array variable
                    for(int i=0; keywords[index+2].words[i] != '\0' && i < WORD_MAX_LENGTH; i++){ // checking if i is under WORD_MAX_LENGTH may seem unnecessary..
                        if( keywords[index+2].words[i] == ',' ){
                            printf("array identifier comma was found at line: %d\n",keywords[index+2].line);
                            char check_buffer[WORD_MAX_LENGTH];
                            for(int buffer_i=0; buffer_i < i; buffer_i++){
                                check_buffer[buffer_i]= keywords[index+2].words[buffer_i]; //copys text value before ',' to buffer from comparison
                            }
                            check_buffer[i]='\0';

                            for(int i=0; i<variable_total_count; i++){
                                if( strcmp(&variable_list[i].name[0], &check_buffer[0]) == 0 ){ // 명령어 줄에 있는 변수 이름이 선언된었는지 확인한다.
                                    executable_list[executable_index].variable_index = i;
                                    variable_list[i].is_array=1;
                                    success_flag =1;
                                    printf("array name: %s\n", &check_buffer[0]);
                                    break;
                                }
                                printf("\n nope %s  %s\n" , &variable_list[i].name[0], &check_buffer[0]);
                            }
                        }
                    }

                }


                if(success_flag ==0){
                    printf("variable name not declared! ERROR line: %d\n",keywords[index+2].line);
                }

                index += 3;
                executable_index++;
            }
            else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line +1  == keywords[index+2].line){ // 다음 두 명령어가 같은 줄에 있고, 다음 명령어는 다음 줄에 있다.
                printf("%d: instruction line without label\n",keywords[index].line);
                executable_list[executable_index].label_index=0;
                executable_list[executable_index].instruction = keywords[index].type;
                bool success_flag=0;
                for(int i=0; i<variable_total_count; i++){
                    //printf("\n %s %s\n",&variable_list[i].name[0],keywords[index+1].words);
                    if( strcmp(&variable_list[i].name[0], keywords[index+1].words) == 0 ){
                        executable_list[executable_index].variable_index = i;
                        success_flag =1;
                        variable_list[i].is_array=0;
                        //printf("suflag: %d\n",success_flag);
                        break;
                    }
                }
                //printf("suflag2: %d\n",success_flag);
                if(success_flag ==0){ // if a normal name was not detected, check if it's array variable
                    for(int i=0; keywords[index+1].words[i] != '\0' && i < WORD_MAX_LENGTH; i++){ // checking if i is under WORD_MAX_LENGTH may seem unnecessary..
                        if( keywords[index+1].words[i] == ',' ){
                            printf("array identifier comma was found at line: %d\n",keywords[index+1].line);
                            char check_buffer[WORD_MAX_LENGTH];
                            for(int buffer_i=0; buffer_i < i; buffer_i++){
                                check_buffer[buffer_i]= keywords[index+1].words[buffer_i]; //copys text value before ',' to buffer fro comparison
                            }
                            check_buffer[i]='\0';


                            for(int i=0; i<variable_total_count; i++){
                                if( strcmp(&variable_list[i].name[0], &check_buffer[0]) == 0 ){ // 명령어 줄에 있는 변수 이름이 선언된었는지 확인한다.
                                    executable_list[executable_index].variable_index = i;
                                    variable_list[i].is_array=1;
                                    success_flag =1;
                                    printf("array name: %s\n", &check_buffer[0]);
                                    break;
                                }
                                //printf("\n nope list:%s  check_buffer:%s result: %d\n" , &variable_list[i].name[0], &check_buffer[0],strcmp(&variable_list[i].name[0],  &check_buffer[0]) );
                            }
                        }
                    }
                }


                if(success_flag ==0){
                    printf("variable name not declared! ERROR line: %d\n",keywords[index+1].line);
                }


                index +=2;
                executable_index++;
            }
    }

    printf("executable count: %d label count: %d variable count: %d\n", executable_index,label_index,variable_total_count);
    int label_total_count = label_index;
    int executable_total_count=executable_index;
    for(int i=0;i<executable_total_count; i++){
        if(executable_list[i].label_index ==0){ //두 줄 짜리 명령어
            printf(" instruction type: %d variable index: %d\n",executable_list[i].instruction,executable_list[i].variable_index);
        }
        else{ //라벨이 있는 세 줄짜리 명령어
            printf("label index: %d index in label: %d  instruction type: %d variable index: %d\n",executable_list[i].label_index,label_list[executable_list[i].label_index-1].to_here,executable_list[i].instruction,executable_list[i].variable_index);
            // label_list[executable_list[i].label_index-1].to_here 에서 보면 executable_list[i].label_index값에 -1이 들어가 있는데, 이는 struct executable가 label_index값이 0일때 라벨이 없다고 판단해서 그렇다.
        }
    }
    //인제 프로그램을 실행하는 코드
    //먼저, 레지스터를 생성한다.
    /*
    char *R_a = make_var(3); //accumulator register
    char *R_x = make_var(3); //index register
    char *R_l = make_var(3); //linkage register
    */
    int PC; // program counter
    //int SW;// status word? 애 이름 제대로 알아내기

    //printf("value! %d", *(variable_list[1].ptr+2*24));

    int *RegisterList[4];
    for(short i=0;i<4;i++){
        RegisterList[i]= make_var(3);
        *RegisterList[i]=0;
    }

    for(short i=0; i<4;i++){
        printf("i: %d address %p value %d\n",i,RegisterList[i],*RegisterList[i]);
    }
    printf("int size: %p\n", sizeof(int));
    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word
    //*(RegisterList[0]+8*sizeof(int)) = 4;
    //*RegisterList[1]=4;

    /*for(short i=0; i<4;i++){
        printf("i: %d address %p value %d\n",i,RegisterList[i],*RegisterList[i]);
    }
    */
    //*R_a = *R_x = *R_l = SW = 0;// reset registers to zero
    PC= 0;
    /*
    for(int array_i =0; array_i< variable_list[2].array_max; array_i++){
        printf("%d:%d ",array_i, *(variable_list[2].ptr+array_i*24) );
    }
    */

    printf("\nSIC code now executes!\n===============================\n");

    for(int i=0;i<executable_total_count;i++){ // now, finally a code that runs everything.

        if(executable_list[i].instruction>0 && executable_list[i].instruction < 7){ // 1~6 are instruction that perform Math calculations on the value.
            MathCalculate(executable_list[i].instruction, &RegisterList[0],&variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>6 && executable_list[i].instruction < 11){ // 7~10 are load instructions
            LoadFunction(executable_list[i].instruction, &RegisterList[0] , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>10 && executable_list[i].instruction < 15){ // 7~10 are load instructions
            StoreFunction(executable_list[i].instruction, &RegisterList[0] , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>14 && executable_list[i].instruction < 17){ //these are compare functions
            CompareFunction(executable_list[i].instruction, &RegisterList[0] , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }

        printf("executed line: %d REGISTER STATUS:\nRa: %d Rx: %d Rl: %d PC: %d SW: %d\n", i+1 ,*RegisterList[0],*RegisterList[1],*RegisterList[2],PC,*RegisterList[3]);
        //printf("rx in loop register address: %p\n", RegisterList[1]);

        printf("VARIABLE STATUS:\n");
        for(int var_i=0; var_i < variable_total_count; var_i++ ){
            //printf(" %d ", variable_list[i].is_array);
            if( variable_list[var_i].is_array == 0){
                printf("%s: %d ",&variable_list[var_i].name[0], *variable_list[var_i].ptr);
            }
            else if( variable_list[var_i].is_array == 1){ // print out the whole array.
                printf("array ! %s: ",&variable_list[var_i].name[0]);
                for(int array_i =0; array_i< variable_list[var_i].array_max; array_i++){
                    struct bit_shield_24 shield_temp;
                    shield_temp.data = *(variable_list[var_i].ptr+array_i*24); // probably not necessary here... but if it was, I need to add bit shields everywhere
                    printf("%d:%d ",array_i, shield_temp.data );
                }
            }
            else{
                printf("variable error!\n");
            }
        }
        //printf("value! %d", *(variable_list[1].ptr+2*24));
        printf("\n\n");

    }
    return 0;
}

void CompareFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word
    // 0 for equal '='
    //1 when register value is Greater than variable value '>'
    // 2 when resgister value is smaller than variable value '<'

    if(instruction==15){ // COMP
        if( *RegisterAddress[0] > *to_variable->ptr) // Store value '>' to SW, will branch when JGT is used
            *RegisterAddress[3] = 1;
        else if( *RegisterAddress[0] < *to_variable->ptr ) // '<'
            *RegisterAddress[3] = 2;
        else if( *RegisterAddress[0] == *to_variable->ptr ) // '='
            *RegisterAddress[3] = 0;
    }
    else{ // TIX
        *RegisterAddress[1]++;
        if( *RegisterAddress[0] > to_variable->ptr[*RegisterAddress[1] ]) // Store value '>' to SW, will branch when JGT is used
            *RegisterAddress[3] = 1;
        else if( *RegisterAddress[0] < to_variable->ptr[*RegisterAddress[1] ] ) // '<'
            *RegisterAddress[3] = 2;
        else if( *RegisterAddress[0] == to_variable->ptr[*RegisterAddress[1] ] ) // '='
            *RegisterAddress[3] = 0;
    }
}

void StoreFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    if(to_variable->is_array == 1){
        if(instruction <13){
            if(instruction == 11) // STA
                *( &to_variable->ptr[to_executable->variable_index]+ *RegisterAddress[1] * 24) = *RegisterAddress[0] ;
        else {// STCH
            //printf("here: %d \n ",RegisterAddress[0]&16776960 );
            // 1111_1111 in binary is 255 or 256?
            to_variable->ptr[to_executable->variable_index] = 255 & *RegisterAddress[0] ; // 255 it is!
        }
    }
        else{
            if(instruction==13){ //STL
                *to_variable->ptr = *RegisterAddress[2] ; // store value from linkage register
            }
            else{ // STX
                *to_variable->ptr = *RegisterAddress[1] ; // store value from index register
            }
        }
    }
    else{
        if(instruction <13){
            if(instruction == 11){ // STA
                struct bit_shield_24 shield_temp;
                shield_temp.data = *RegisterAddress[0];
                *to_variable->ptr =  shield_temp.data;
            }
        else {// STCH
            //printf("here: %d \n ",RegisterAddress[0]&16776960 );
            // 1111_1111 in binary is 255 or 256?
            *to_variable->ptr = 255 & *RegisterAddress[0] ; // 255 it is!
        }
    }
        else{
            if(instruction==13){ //STL
                *to_variable->ptr = *RegisterAddress[2] ; // store value from linkage register
            }
            else{ // STX
                printf("rx register address: %p in STL\n",&RegisterAddress[1]);
                *to_variable->ptr = *RegisterAddress[1] ; // store value from index register
            }
        }
    }

}

void LoadFunction(int instruction,int **RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word
    if( to_variable->is_array == 1){ //variable is array
        printf("load array! in load function %p \n");
        int mem_size = 24;
        printf("data type %d ",to_variable->data_type);
        if(to_variable->data_type==1)// char type
            mem_size = 8;

        if(instruction<9){
        if(instruction == 7){ // LDA
            struct bit_shield_24 shield_temp;
            shield_temp.data= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
            *RegisterAddress[0] = shield_temp.data;
        }
        else{ // LDCH
            // bit mask 0b 1111_1111 to get char value only
            // lets hope c uses small edian...
            // the binary value above is 255
            *RegisterAddress[0] = 255 & *(to_variable->ptr+*RegisterAddress[1]*mem_size); // it works!
        }
    }
        else{
            if(instruction == 9) // LDL
                *RegisterAddress[2] = *(to_variable->ptr+*RegisterAddress[1]*mem_size);
            if(instruction == 10){ // LDX
                *RegisterAddress[1] = *(to_variable->ptr+*RegisterAddress[1]*mem_size);
                //printf("Rx address in ldx function %d",&RegisterAddress[1]);
            }
        }
    }
    else{ // variable is not a array
        if(instruction<9){
        if(instruction == 7) // LDA
            *RegisterAddress[0] = *to_variable->ptr;
        else{ // LDCH
            // bit mask 0b 1111_1111 to get char value only
            // lets hope c uses small edian...
            // the binary value above is 255
            *RegisterAddress[0] = 255 & *to_variable->ptr; // it works!
        }
    }
        else{
            if(instruction == 9) // LDL
                *RegisterAddress[2] = *to_variable->ptr;
            if(instruction == 10) // LDX
                *RegisterAddress[1] = *to_variable->ptr;
        }
    }

}

void MathCalculate(int instruction,int **RegisterAddress,struct variable *to_variable, struct executable * to_executable){
    if(to_variable->is_array == 1){ //variable is array
        int mem_size = 24;
        if(to_variable->data_type==1)// char type
            mem_size = 8;

        if(instruction<4){
            if(instruction==1){ //ADD
                //printf("magic! Ra: %d variable value: %d",**Register_A,**to_variable->ptr)
                *RegisterAddress[0] += *(to_variable->ptr+*RegisterAddress[1]*mem_size);
            }
        else if(instruction==2) // SUB
            *RegisterAddress[0] -= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
        else if(instruction==3) //MUL
            *RegisterAddress[0] *= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
        }
        else{
            if(instruction==4) //DIV
                *RegisterAddress[0] /= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
            else if(instruction==5) // AND
                *RegisterAddress[0] &= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
            else if(instruction==6) // OR
                *RegisterAddress[0] |= *(to_variable->ptr+*RegisterAddress[1]*mem_size);
        }
    }
    else{ // variable is not array
        if(instruction<4){
            if(instruction==1){ //ADD
                //printf("magic! Ra: %d variable value: %d",**Register_A,**to_variable->ptr)
                *RegisterAddress[0] += *to_variable->ptr;
            }
        else if(instruction==2) // SUB
            *RegisterAddress[0] -= *to_variable->ptr;
        else if(instruction==3) //MUL
            *RegisterAddress[0] *= *to_variable->ptr;
        }
        else{
            if(instruction==4) //DIV
                *RegisterAddress[0] /= *to_variable->ptr;
            else if(instruction==5) // AND
                *RegisterAddress[0] &=*to_variable->ptr;
            else if(instruction==6) // AND
                *RegisterAddress[0] |= *to_variable->ptr;
        }
    }

}

int * make_var(int how_many_word){ // SIC 머신에서 한 워드가 8비트이다. 레지스터는 모두 3워드, 24비트 이다.
    int *pointer;
    pointer = (int*)malloc(how_many_word*8);
    return pointer;
}

bool num_check(char *address){
    for(int i=0; address[i] != '\0'; i++)
        if(isdigit( address[i])==0 ) // isdigit 값이 0이면 해당 값이 숫자가 아니라는 소리이다
            return 0;
    return 1;
}

short data_check(char *address2){  //return 1 for pure num, 2 for char,3 for hex,
    if(num_check( address2) )
        return 1;
    else if(address2[1]== '\'' && address2[3]== '\''){
        if(address2[0] =='C' )
            return 2;
        if(address2[0]=='H' )
            return 3;
    }
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
