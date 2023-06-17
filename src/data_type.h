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
    struct bit24* ptr; // for char variables, the 8bit memory size is "emulated"
    bool is_array;
    int array_max;
    bool data_type;
};

struct executable{
    int instruction;
    int variable_index;
    int label_index; // points toward the label's index in the label list
    bool has_label; // identifier for instructions with labels, used to check if the junction is indeed jumping to a valid labeled line, and also used when printing the instrucions
};

struct label{
    char name[WORD_MAX_LENGTH];
    int to_here;
    bool valid; // checks if to_here was actually updated, thus checking if this label was actully declared and is pointing somewhere valid
};

struct bit24{
    unsigned int data : 24;
};

struct bit8{
    unsigned int data : 8;
};

bool num_check(char *address);
short data_check(char *address2);
bool isJunction(int type);
void var_linker(struct variable *var_list, const int total_var, struct Word * var_name, struct executable * ex_list);


struct bit24 * make_var(int how_many_word);

void MathCalculate(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void LoadFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void StoreFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable);
void CompareFunction(int instruction,struct bit24 *RegisterAddress,struct variable *to_variable,struct executable * to_executable);
// void IOFunction(int instruction,int *Register_A);

void error(int error_mode);
