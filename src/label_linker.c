            bool label_already_exist=0;

            printf("%d: instruction line with label\n",keywords[index].line);
            for(int i=0; i< how_many_label_so_far; i++){ // !TODO
                if(strcmp(&label_list[i].name[0], keywords[index].words ) == 0){ //라벨 이미 있음
                    //strcpy(&label_list[label_index].name[0], keywords[index].words);  //라벨 저장
                    label_list[label_index].to_here = executable_index; // 라벨 위치 지정
                    label_index++;
                }
            }

            if(label_already_exist==0){
                strcpy(&label_list[label_index].name[0], keywords[index].words);  //라벨 저장
                label_list[label_index].to_here = executable_index; // 라벨 위치 지정
                label_index++;
                how_many_label_so_far++;
            }

bool var_linker(struct variable *var_list, const int total_var, struct Word * var_name, struct executable * ex_list);
