
## 명령어 디코더
파일의 내용을 램에 저장한 다음 건드는 것이 나을까?
아니면 입력 받는 즉시 명령어로 읽어들여서 배열에 저장할까?


# 전체적인 알고리즘

## 파일 읽기
파일을 읽어들여서 스페이스가 나오면 그냥 무시하고, 텍스트가 나오면 다음 스페이스가 나올때까지 문자를 읽어 들인 다음, 미리 저장된 명령어 중에 일치하는 것이 있는지 찾아본다.
일치하지 않으면 에러 메세지를 보여주고 에셈블링을 아예 하지 않는다.

## isalnum
defined in ctype.h
소문자, 대문자, 숫자 입력을 받으면 0이 아닌 값을 리턴한다.
스페이스나 \n같은 문자를 입력 받으면 0을 리턴한다.

## 프로그래밍 메모
strcmp 함수 설명 p562
서로 같은 문자열이면 0을 리턴한다.
문자열 배열 p527
사용자 정의 출력 p548

## 변수 읽기
변수의 이름은 영어 또는 숫자가 포함될 수도 있으니 변수 이름을 읽어 드릴 때 이를 조심해야 한다.

## 알고리즘 ver2
스체이스와 개행 문자로 '\n' 나누어진 워드들을 모두 읽어들이고, 이 워드가 명령어인지 아닌지, 몇번째 줄에 있는 지 등의 정보를 구조체에 저장한다.
구조체 배열들을 순서대로 저장하고, 구조체 배열을 읽는 함수를 만들어 구조체들이 명령어인지, 변수 인지, 라벨인지 구분한다.
## fopen() 알아보기


            while(instruction_buffer[instruc_char_index] !='\0'){
                putchar(instruction_buffer[instruc_char_index]);
                instruc_char_index++;
            }

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

       printf("%s에는 %ld개의 문자, %ld개의 대문자가 들어 있습니다.\n",argv[1],count,Upper);

