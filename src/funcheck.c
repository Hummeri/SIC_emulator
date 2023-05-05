#include<stdio.h>
#include<ctype.h>


int main(){
    printf("%d", isalnum('a'));
    printf("%d",isalnum('A'));
    printf("%d",isalnum('1'));
    printf("%d",isalnum('\n'));
    printf("%d",isalnum(' '));
}
