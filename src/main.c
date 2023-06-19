#include<stdio.h>
#include<stdlib.h> //used for fopen and mallac() and free()
#include<string.h> //used for strcmp() and strcmp()
#include<ctype.h> //used for isupper()
#include<stdbool.h> //for bool flags
#include"feature.h" // all functions are declared here

int main(int argc,char *argv[]){
    int ch; //왜 ch은 int형인가?
    FILE *fp;
    unsigned long count =0;
    unsigned long Upper =0;
    char program_name[256];

    //프로그램이름+입력된 파일이름 말고도 파일이름이 하나 추가될때 이런 에러가 나타난다.
    if(argc!=2) {  // FEATURE! add more custom arguments like quiet output.
        printf("usage: %s filename\n",argv[0] );
        exit(EXIT_FAILURE); //얘는 stdlib에서 정의된다
    }

    if( (fp = fopen(argv[1],"r")) == NULL ){
        printf("failed to open fail %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }


    struct Word Program[MAX_PROGRAM_INSTRUCTIONS];
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
        else if( (ch==' ' || ch=='\t') && not_empty_space ){ //ch가 스페이스 일때,
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

    fclose(fp); //얘를 안쓰면 어떻게 되지?

    putchar('\n');
    assembler(&Program[0],Word_count);

    return 0;
} //end of main


int assembler(struct Word *keywords,int index_max){
    //title and start and memory start address check
    char title[WORD_MAX_LENGTH];

    if(keywords[0].type == 0){
        if(strcmp(keywords[1].words,"START")==0 && keywords[0].line == keywords[1].line)
            if(keywords[1].line == keywords[2].line  && num_check( &keywords[2].words[0]) ){
                printf("valid start\n");
                strcpy(&title[0],keywords[0].words);
                //printf("%c\n",title[0]);
            }
    }
    else{
        printf("not valid start\n");
    }
    int index =3;

    while( index < index_max) {
        if(keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line && keywords[index+2].line == keywords[index+3].line){ // 같은 줄에 키워드가 네개 에러 발생!
            printf("instruction format error at line: %d more than three keywords\n", keywords[index].line);
            error(0);
        }
        else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line){ // 다음 명령어 세개가 같은 줄에 있다.
            printf("instruction line with three arguments\n");
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
        error(3);
    }

    int var_index_max = index; // 변수 배열을 생성하는 코드
    index = var_index_start;
    //printf("words:%s: type: %d, line: %d\n",keywords[index].words, keywords[index].type , keywords[index].line);

    while(index<var_index_max){
        if( keywords[index].line == keywords[index+1].line && keywords[index+1].line== keywords[index+2].line
&& keywords[index].type == 0 && keywords[index+1].type > 25 && keywords[index+1].type <= 30 && data_check(&keywords[index+2].words[0])){
            printf("valid variable\n");
            strcpy( variable_list[var_index].name, keywords[index].words);

            if( keywords[index+1].type == 27) { //변수 타입 BYTE 얘는 CHAR1 BYTE C'Z' 같은 형식을 사용할 수 있으니 저 c와 '를 검사하는 걸 넣어야한다.


                variable_list[var_index].ptr = make_var(MAX_ARRAY_SIZE);
                variable_list[var_index].ptr->data=0;
                variable_list[var_index].data_type = 1;
                printf("line 213!!!!\n");

                int string_end=0;
                if( keywords[index+2].words[0] == 'c'){
                    printf("char type indicator c detected\n");
                    if( keywords[index+2].words[1] == '\'' ){
                        for(int i=2; keywords[index+2].words[i] != '\0'; i++){
                            if(keywords[index+2].words[i] == '\''){

                                string_end=i;
                                printf("end index found %d",string_end);
                                break;
                            }
                        }
                        if( string_end >3 ){ // it's a string array!
                            variable_list[var_index].array_max= string_end-2;

                            variable_list[var_index].is_array = 1;


                            for(int i=0; i< variable_list[var_index].array_max ; i++){
                                char temp = keywords[index+2].words[i+2];
                                printf("!%d.%c!\n",i,keywords[index+2].words[i+2]);
                                (variable_list[var_index].ptr + sizeof(struct bit24)*i)->data = temp;
                            }

                        }
                        else{ // not a string array
                            char temp ;
                            printf("!%c!\n",keywords[index+2].words[2]);
                            temp= keywords[index+2].words[2];
                            variable_list[var_index].ptr->data = temp;
                            variable_list[var_index].is_array = 0;
                        }
                    }
                    else{
                        error(2);
                    }
                }
                else{
                    error(1);
                }



            printf("BYTE: name:%s value: %c is_array: %d\n",variable_list[var_index].name ,variable_list[var_index].ptr->data,variable_list[var_index].is_array);
            }
            else if(keywords[index+1].type == 28){  //변수 타입 WORD
                variable_list[var_index].data_type = 0;

                int comma_count =0;
                for(int i =0; keywords[index+2].words[i] != '\0' && i < WORD_MAX_LENGTH; i++){
                    if(keywords[index+2].words[i] == ',')
                        comma_count++;
                }
                if(comma_count==0){ // just a normal WORD variable
                    printf("\nmade variable\n");
                    variable_list[var_index].ptr = make_var(1);
                    (variable_list[var_index].ptr)->data = atoi(&keywords[index+2].words[0]); //the atoi function requires const str* type, but somehow still works
                    variable_list[var_index].is_array=0;
                }
                else{ // a WORD array variable
                    printf("\nmade array\n");
                    variable_list[var_index].ptr = make_var( MAX_ARRAY_SIZE ); //calloc(comma_count+1,sizeof(struct bit24));
                    char buffer[WORD_MAX_LENGTH];
                    int pointer_offset =0;
                    int buffer_index=0;

                    for(int i=0; i < WORD_MAX_LENGTH; i++){
                        if(keywords[index+2].words[i] == ','){
                            buffer[buffer_index]='\0';
                            printf("that string is %d as a number ptr_offset: %d\n ",atoi(&buffer[0]), pointer_offset );
                            struct bit24 bit_shield;
                            bit_shield.data = atoi(&buffer[0]);

                            (variable_list[var_index].ptr + sizeof(struct bit24)*pointer_offset )->data= bit_shield.data;
                            // *(variable_list[var_index].ptr+ pointer_offset*24 )= shield_temp.data;
                            buffer_index=0;
                            pointer_offset++;
                            continue;
                        }
                        else if(keywords[index+2].words[i] == '\0' /*|| pointer_offset == comma_count*/){
                            buffer[buffer_index]='\0';
                            printf("buffer last : %s ptr_offset: %d \n",&buffer[0], pointer_offset);

                            printf("that string is %d as a number\n ",atoi(&buffer[0]) );
                            struct bit24 bit_shield;
                            bit_shield.data = atoi(&buffer[0]);

                            (variable_list[var_index].ptr + sizeof(struct bit24)*pointer_offset )->data = bit_shield.data;
                            printf("value here: %d\n",(variable_list[var_index].ptr+pointer_offset*sizeof(struct bit24))->data);

                            variable_list[var_index].is_array=1;
                            variable_list[var_index].array_max = comma_count +1;
                            break;
                        }
                        else{
                            buffer[buffer_index]=keywords[index+2].words[i];
                            buffer_index++;
                        }
                    }
                }
                printf("WORD, name:%s value: %d is_array:%d var_index: %d\n",variable_list[var_index].name ,(variable_list[var_index].ptr)->data,variable_list[var_index].is_array,var_index);
            }
            else if(keywords[index+1].type == 29) { //RESB
                int temp = atoi(&keywords[index+2].words[0]);
                variable_list[var_index].ptr = make_var( MAX_ARRAY_SIZE);
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
                int temp = atoi(&keywords[index+2].words[0]);
                variable_list[var_index].ptr = make_var( MAX_ARRAY_SIZE );
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
    const int variable_total_count = var_index; //TODO2 if too many variables, exit with error.
    printf("variable_total_count: %d \n=====================\n", variable_total_count);


    index =3;

        // 명령어 집합하기
    struct label label_list[MAX_LABEL_COUNT];
    struct executable executable_list[MAX_PROGRAM_INSTRUCTIONS];
    int executable_index=0;
    int label_count=0;
    // int how_many_label_so_far=0;

    //printf("%d\n",index_max);
    while( index < var_index_start) {
        // printf("here label_count value is : %d\n", label_count);
        if( keywords[index].line == keywords[index+1].line && keywords[index+1].line == keywords[index+2].line){ // 다음 명령어 세개가 같은 줄에 있다.
            // executable_list[executable_index].label_index= label_index;
            printf("%d: instruction line with label\n",keywords[index].line);
            executable_list[executable_index].instruction = keywords[index+1].type;

            if(isJunction(keywords[index].type) ){
                printf(" instruction is junction type\n");
                label_linker(label_list, &label_count, &keywords[index], &executable_list[executable_index], executable_index, 1, 1);
            }
            else{
                printf("instruction is not junction type\n");
                label_linker(label_list, &label_count, &keywords[index], &executable_list[executable_index], executable_index, 0, 1);
                var_linker(variable_list,variable_total_count,&keywords[index+2],&executable_list[executable_index]);

            }



            index += 3;
            executable_index++;
        }
        else if( keywords[index].line == keywords[index+1].line && keywords[index+1].line +1  == keywords[index+2].line){ // 다음 두 명령어가 같은 줄에 있고, 다음 명령어는 다음 줄에 있다.
            printf("%d: instruction line without label\n",keywords[index].line);
            executable_list[executable_index].instruction = keywords[index].type;

            if( isJunction(keywords[index].type) ){ // TODOed if the instruction is a junction instruction, than the second argument is not a variable, but a label
                printf(" instruction is junction type\n");
                label_linker(label_list, &label_count, &keywords[index], &executable_list[executable_index], executable_index, 1, 0);

            }
            else{ // not a junction instruction
                printf("instruction is not junction type\n");
                var_linker( variable_list ,variable_total_count, &keywords[index+1], &executable_list[executable_index]);
            }

            index +=2;
            executable_index++;
        }
        else{
            error(4);// program instruction mismatch error!
        }
    }



    printf("executable count: %d label count: %d variable count: %d\n", executable_index,label_count,variable_total_count);
    const int label_total_count = label_count;
    const int executable_total_count=executable_index;


    for(int i=0;i<executable_total_count; i++){
        printf(": %d",i);
        if( isJunction(executable_list[i].instruction) ){
            printf(" instruction type: %d label index: %d to_here value: %d \n",executable_list[i].instruction,executable_list[i].label_index,label_list[executable_list[i].label_index].to_here);
        }
        else{
            printf(" instruction type: %d variable index: %d\n",executable_list[i].instruction,executable_list[i].variable_index);
        }

    }
    printf("=================label_list\n");
    for(int i=0; i< label_total_count; i++){
        printf("label name: %s label index: %d to_here: %d\n",label_list[i].name,i,label_list[i].to_here);
    }



    char ch = getchar(); // make program stop here.

    //인제 프로그램을 실행하는 코드
    //먼저, 레지스터를 생성한다.
    int PC; // program counter
    int return_index=0; // for J SUB and RSUB

    struct bit24 *RegisterList= make_var(4);

    for(short i=0;i<4;i++){

        (RegisterList+sizeof(struct bit24)*i)->data= 0;
    }

    printf("register default value\n");
    for(short i=0; i<4;i++){
        printf("i: %d address %p value %d\n",i,(RegisterList+sizeof(struct bit24)*i),(RegisterList+sizeof(struct bit24)*i)->data);
    }


    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word
    PC= 0;


    printf("\nSIC code now executes!\n===============================\n");

    for(int i=0;i<executable_total_count;i++){ // now, finally a code that runs everything.

        if(executable_list[i].instruction>0 && executable_list[i].instruction < 7){ // 1~6 are instruction that perform Math calculations on the value.
            MathCalculate(executable_list[i].instruction, RegisterList,&variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>6 && executable_list[i].instruction < 11){ // 7~10 are load instructions
            LoadFunction(executable_list[i].instruction, RegisterList , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>10 && executable_list[i].instruction < 15){ // 11~14 are  instructions
            StoreFunction(executable_list[i].instruction, RegisterList , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>14 && executable_list[i].instruction < 17){ //15,16 are compare functions
            CompareFunction(executable_list[i].instruction, RegisterList , &variable_list[executable_list[i].variable_index],&executable_list[i]);
        }
        else if(executable_list[i].instruction>16 && executable_list[i].instruction < 23){ // finally, junction functions! 17~22
            if(executable_list[i].instruction == 17){ // J
                i=label_list[executable_list[i].label_index].to_here;
            }
            else if(executable_list[i].instruction == 18){ // JEQ
                if( (RegisterList+sizeof(struct bit24)*3)->data == 0){
                    i=label_list[executable_list[i].label_index].to_here;
                }
            }
            else if(executable_list[i].instruction == 19){ // JGT
                if( (RegisterList+sizeof(struct bit24)*3)->data == 1){
                    i=label_list[executable_list[i].label_index].to_here;
                }
            }
            else if(executable_list[i].instruction == 20){ // JLT
                if( (RegisterList+sizeof(struct bit24)*3)->data == 2){
                    i=label_list[executable_list[i].label_index].to_here;
                }
            }
            else if(executable_list[i].instruction == 21){ // J SUB
                    i=label_list[executable_list[i].label_index].to_here;
                    return_index=i;
                }
                else{ // R SUB
                    i= return_index;
                }
                printf("label index: %d label line index %d\n",label_list[executable_list[i].label_index],label_list[executable_list[i].label_index].to_here);
            }

        printf("executed line: %d REGISTER STATUS:\nRa: %d Rx: %d Rl: %d PC: %d SW: %d\n", i/*+1*/ ,(RegisterList+sizeof(struct bit24)*0)->data,(RegisterList+sizeof(struct bit24)*1)->data,(RegisterList+sizeof(struct bit24)*2)->data,PC,(RegisterList+sizeof(struct bit24)*3)->data);

        printf("VARIABLE STATUS:\n");
        for(int var_i=0; var_i < variable_total_count; var_i++ ){
            // printf(" isarray:%d ", variable_list[var_i].is_array);
            if( variable_list[var_i].is_array == 0){
                if(variable_list[var_i].data_type ==1){ //if data is char
                    /*
                     variable_list[var_i].ptr->data = 'c';
                     struct bit24 size_emulation;
                     size_emulation.data = 255;
                     */
                     char output = variable_list[var_i].ptr->data;
                    printf("%s: %d : %c\n",&variable_list[var_i].name[0], variable_list[var_i].ptr->data,output);
                }
                else{
                    printf("%s: %d\n",&variable_list[var_i].name[0], variable_list[var_i].ptr->data);
                }
            }
            else if( variable_list[var_i].is_array == 1){ // print out the whole array.
                printf("array %s: ",&variable_list[var_i].name[0]);
                for(int array_i =0; array_i< variable_list[var_i].array_max; array_i++){

                    if(variable_list[var_i].data_type ==1){
                        char output = (variable_list[var_i].ptr+array_i*sizeof(struct bit24))->data;
                        printf("index:%dvalue:%d:%c ",array_i,(variable_list[var_i].ptr+array_i*sizeof(struct bit24))->data,output);
                    }
                    else{
                        printf("index:%dvalue:%d ",array_i, (variable_list[var_i].ptr+array_i*sizeof(struct bit24))->data );
                    }

                }
                putchar('\n');
            }
            else{
                printf("variable error!\n");
            }
        }
        printf("\n");
        // char car = getchar();
    }
    return 0;
}

bool isJunction(int type){ // junction functions are type defined as 17~22 value from struct keyword
    if(type>16 && type < 23)
        return 1;
    else
        return 0;
}

void label_linker(struct label *label_list,int * label_count ,struct Word * keyWord, struct executable * ex_list,const int executable_index,bool junction_mode,bool argument_mode){
    // printf("got label_count value is : %d\n", *label_count);


    if(argument_mode == 1){ // if three worded argument, update label list and make label point this
        // pass first keyword to keyWord
        // than, access lebels by keyWord,  keyWord[2]

        bool label_found =0;

        for(int i=0;i< *label_count;i++){ // look for label
            if(strcmp(label_list[i].name, keyWord->words )==0){ // label found!
                label_found=1;
                label_list[i].to_here= executable_index; // if label was found,it means label was already declared. thus, just update its to_here value
                printf("!!exe index: %d\n", executable_index);
            }
        }

        if(label_found == 0){ // label was not found, make new label
            if(*label_count+1  > MAX_LABEL_COUNT){
                printf("error at line %d\n",keyWord->line);
                error(6);
            }
            printf("\n=====%s=======\n",keyWord->words);
            strcpy(label_list[*label_count].name,keyWord->words);
            label_list[*label_count].to_here = executable_index;
            label_list[*label_count].valid =1;
            *label_count += 1;
            printf(" label_count value is : %d\n", *label_count);
            printf("exe index: %d\n", executable_index);
            // printf(" label_count value is : %d %d %d\n", *label_count,ex_list[executable_index].label_index, label_list[ex_list[executable_index].label_index].to_here );
        }
    }

    if(junction_mode == 1){ // if command is a junction command
        bool label_found =0;
        for(int i=0;i< *label_count; i++){ // look for label
            printf("strcmp value: %s:%s %d\n",label_list[i].name,keyWord[1+argument_mode].words, strcmp(label_list[i].name, keyWord[1+argument_mode].words ) );

            if(strcmp(label_list[i].name, keyWord[1+argument_mode].words )==0 ){ // keyWord[1+argument_mode] this code makes the keyWord point towards the label argument in both three and two argument cases
                label_found = 1;
                ex_list->label_index=i;
                printf("ex got label index %d \n",ex_list->label_index);

            }
        }
        if(label_found == 0){ // make new label, but don't update to_here value. just copy string
            if(*label_count+1  > MAX_LABEL_COUNT){ // check if adding a new label will cause overflow
                printf("error at line %d\n",keyWord->line);
                error(6);
            }

            printf("\n%s\n",keyWord[1+argument_mode].words);
            strcpy(label_list[*label_count].name,keyWord[1+argument_mode].words);
            ex_list->label_index = *label_count;
            *label_count +=1;
            printf(" label_count value is : %d ex_list.label_index: %d label_index.to_here %d\n", *label_count,ex_list->label_index, label_list[ex_list->label_index].to_here );
        }
    }

}


void var_linker(struct variable *var_list, const int total_var, struct Word * keyWord, struct executable * ex_list){
    bool success_flag=0;
    for(int i=0; i< total_var; i++){
        if( strcmp(&var_list[i].name[0], keyWord->words) == 0 ){
            ex_list->variable_index = i;
            success_flag =1;
            break;
        }
    }
    if(success_flag ==0){ // if a normal name was not detected, check if it's array variable
        for(int i=0; keyWord->words[i] != '\0' && i < WORD_MAX_LENGTH; i++){
            if( keyWord->words[i] == ',' ){
                printf("array identifier comma was found at line: %d\n",keyWord->line);
                char check_buffer[WORD_MAX_LENGTH];
                for(int buffer_i=0; buffer_i < i; buffer_i++){
                    check_buffer[buffer_i]= keyWord->words[buffer_i]; //copys text value before ',' to buffer fro comparison
                }
                check_buffer[i]='\0';

                for(int i=0; i<total_var; i++){
                    if( strcmp( &var_list[i].name[0], &check_buffer[0]) == 0 ){ // 명령어 줄에 있는 변수 이름이 선언된었는지 확인한다.
                        ex_list->variable_index = i;
                        success_flag =1;
                        printf("array name: %s\n", &check_buffer[0]);
                        break;
                    }

                }
            }
        }
    }


    if(success_flag ==0){
        printf("variable name not declared! ERROR line: %d\n",keyWord->line);
        error(5);
    }

}



void CompareFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word
    // 0 for equal '='
    //1 when register value is Greater than variable value '>'
    // 2 when resgister value is smaller than variable value '<'
    struct bit24 size_emulation;
    if(to_variable->data_type==1)// char type
        size_emulation.data = 255;
    else //int type
        size_emulation.data = 16777215;

    struct bit24 offset;
    offset.data=0;

    if(to_variable->is_array == 1){ //variable is array
        offset.data=(RegisterAddress+sizeof(struct bit24)*1)->data;
    }

    if(instruction==15){ // COMP
        if( (RegisterAddress)->data > (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // Store value '>' to SW, will branch when JGT is used
            (RegisterAddress+sizeof(struct bit24)*3)->data = 1;
        else if( (RegisterAddress)->data < (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // '<'
            (RegisterAddress+sizeof(struct bit24)*3)->data = 2;
        else if( (RegisterAddress)->data == (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // '='
            (RegisterAddress+sizeof(struct bit24)*3)->data = 0;
    }
    else{ // TIX
        (RegisterAddress+sizeof(struct bit24)*1)->data++;
        if( (RegisterAddress)->data > (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // Store value '>' to SW, will branch when JGT is used
            (RegisterAddress+sizeof(struct bit24)*3)->data = 1;
        else if( (RegisterAddress)->data < (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // '<'
            (RegisterAddress+sizeof(struct bit24)*3)->data = 2;
        else if( (RegisterAddress)->data == (size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data) ) // '='
            (RegisterAddress+sizeof(struct bit24)*3)->data = 0;
    }
}


void StoreFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    struct bit24 size_emulation;
    if(to_variable->data_type==1)// char type
        size_emulation.data = 255;
    else //int type
        size_emulation.data = 16777215;

    struct bit24 offset;
    offset.data=0;

    if(to_variable->is_array == 1){ //variable is array
        offset.data=(RegisterAddress+sizeof(struct bit24)*1)->data;
    }

        if(instruction <13){
            if(instruction == 11){ // STA
                (to_variable->ptr + offset.data*sizeof(struct bit24) )->data = size_emulation.data & (RegisterAddress)->data;
            }
            else {// STCH
                //printf("here: %d \n ",RegisterAddress[0]&16776960 );
                // 1111_1111 in binary is 255 or 256?
                (to_variable->ptr + offset.data*sizeof(struct bit24) )->data = 255 & (RegisterAddress)->data; // 255 it is!
            }
        }
            else{
                if(instruction==13){ //STL
                    (to_variable->ptr + offset.data*sizeof(struct bit24) )->data =  size_emulation.data & (RegisterAddress+sizeof(struct bit24)*2)->data; // store value from linkage register
                }
                else{ // STX
                    (to_variable->ptr + offset.data*sizeof(struct bit24) )->data =  size_emulation.data & (RegisterAddress+sizeof(struct bit24)*1)->data; // store value from index register
                }
            }



}


void LoadFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable){
    struct bit24 size_emulation;
    if(to_variable->data_type==1)// char type
        size_emulation.data = 255;
    else //int type
        size_emulation.data = 16777215;

    struct bit24 offset;
    offset.data=0;

    if(to_variable->is_array == 1){ //variable is array
        offset.data=(RegisterAddress+sizeof(struct bit24)*1)->data;
    }

    // RegisterList 0 is accumulator register, 1 is index register, 2 is linkage register, 3 is status word

    if(instruction<9){
        if(instruction == 7){ // LDA
            (RegisterAddress)->data = size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data ;
        }
        else{ // LDCH
            // bit mask 0b 1111_1111 to get char value only
            // lets hope c uses small edian...
            // the binary value above is 255
            (RegisterAddress)->data = 255 &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data;
        }
    }
    else{
        if(instruction == 9){ // LDL
            (RegisterAddress+sizeof(struct bit24)*2)->data = size_emulation.data &(to_variable->ptr +offset.data*sizeof(struct bit24) )->data ;
        }
        if(instruction == 10){ // LDX
            (RegisterAddress+sizeof(struct bit24)*1)->data = size_emulation.data &(to_variable->ptr +offset.data*sizeof(struct bit24) )->data ;
        }
    }

}


void MathCalculate(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable, struct executable * to_executable){
    struct bit24 size_emulation;
    if(to_variable->data_type==1)// char type
        size_emulation.data = 255;
    else //int type
        size_emulation.data = 16777215;

    struct bit24 offset;
    offset.data=0;

    if(to_variable->is_array == 1){ //variable is array
        offset.data=(RegisterAddress+sizeof(struct bit24)*1)->data;
    }

        if(instruction<4){
            if(instruction==1){ //ADD
                //printf("magic! Ra: %d variable value: %d",**Register_A,**to_variable->ptr)
                (RegisterAddress)->data +=(
                size_emulation.data &(to_variable->ptr + offset.data*sizeof(struct bit24) )->data );

                //(variable_list[var_i].ptr+array_i*sizeof(struct bit24))->data
            }
        else if(instruction==2) // SUB
            (RegisterAddress)->data -= ( size_emulation.data & (to_variable->ptr + offset.data*sizeof(struct bit24) )->data );
        else if(instruction==3) //MUL
            (RegisterAddress)->data *= ( size_emulation.data & (to_variable->ptr + offset.data*sizeof(struct bit24) )->data );
        }
        else{
            if(instruction==4) //DIV
                (RegisterAddress)->data /= ( size_emulation.data & (to_variable->ptr + offset.data*sizeof(struct bit24) )->data );
            else if(instruction==5){ // AND
                (RegisterAddress)->data &= ( size_emulation.data & (to_variable->ptr + offset.data*sizeof(struct bit24) )->data );
            }
            else if(instruction==6) // OR
                (RegisterAddress)->data |= ( size_emulation.data & (to_variable->ptr + offset.data*sizeof(struct bit24) )->data );
        }

}

struct bit24 * make_var(int how_many_word){ // how_many_word 24비트 저장소 몇개?
    struct bit24 *pointer;
    pointer = malloc(how_many_word* sizeof(struct bit24));
    return pointer;
}

void error(int error_mode){
    switch(error_mode){
        case 0:
            printf("4 instructions error\n");
            break;
        case 1:
            printf("no char indicator 'c' detected\n");
            break;
        case 2:
            printf("no char start indicator \' detected\n");
            break;
        case 3:
            printf("ERROR no END keyword found!\n");
            break;
        case 4:
            printf("program instruction mismatch error! instruction is not aligned properly\n");
            break;
        case 5:
            printf("variable name was not found\n");
            break;
        case 6:
            printf("Label exceeded MAX_LABEL_COUNT VALUE!\n current MAX_LABEL_COUNT value is %d\n",MAX_LABEL_COUNT);
            break;
    }
    // printf("reached here\n");
    exit(error_mode);
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
