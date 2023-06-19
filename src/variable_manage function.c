bool var_linker(struct variable *var_list, const int total_var, struct Word * keyWord, struct executable * ex_list){

var_manage

                bool success_flag=0;
                for(int i=0; i<variable_total_count; i++){
                    if( strcmp(&variable_list[i].name[0], keywords[index+2].words) == 0 ){ // 명령어 줄에 있는 변수 이름이 선언된었는지 확인한다.
                        executable_list[executable_index].variable_index = i;
                        // variable_list[i].is_array=0;
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
                                    // variable_list[i].is_array=1;
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

