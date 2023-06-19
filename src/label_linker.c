            bool label_already_exist=0;


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





void label_linker(struct label *label_list,int * label_count ,struct Word * keyWord, struct executable * ex_list,const int executable_index,bool junction_mode,bool argument_mode){


    if(argument_mode == 1){ // if three worded argument, update label list and make label point this
        // pass first keyword to keyWord
        // than, access lebels by keyWord,  keyWord[2]

        bool label_found =0;

        for(int i=0;i< *label_count;i++){ // look for label
            if(strcmp(label_list[i].name, keyWord.words )==0){ // label found!
                label_found=1;
                label_list[i].to_here= executable_index; // if label was found,it means label was already declared. thus, just update its to_here value
            }
        }

        if(label_found == 0){ // label was not found, make new label
            if(*label_count+1  > MAX_LABEL_COUNT){
                printf("error at line %d\n",keyWords.line);
                error(6);
            }

            strcpy(label_list[*label_count].name,keyWord.words);
            label_list[*label_count].to_here = executable_index;
            label_list[*label_count].valid =1;
            *label_count++;
        }
    }

    if(junction_mode == 1){ // if command is a junction command
        bool label_found =0;
        for(int i=0;i< *label_count; i++){ // look for label
            if(strcmp(label_list[i].name, keyWords[1+argument_mode] )==0 ){ // keyWord[1+argument_mode] this code makes the keyWord point towards the label argument in both three and two argument cases
                label_found = 1;
                ex_list[excutable_index].label_index=i;
                // ex_list[excutable_index].three_arguments=
            }
        }
        if(label_found == 0){ // make new label, but don't update to_here value. just copy string
            if(*label_count+1  > MAX_LABEL_COUNT){ // check if adding a new label will cause overflow
                printf("error at line %d\n",keyWords.line);
                error(6);
            }

            strcpy(label_list[*label_count].name,keyWord.words);
            ex_list[excutable_index].label_index = *label_count;
            *label_count++;
        }



    }

}
