#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../asm_to_binary/asm_to_binary.h"
#include "../binary_to_result/binary_to_result.h"

#define MAX_ASSEMBLY_SIZE 1024  // アセンブリコードの最大サイズ
#define MAX_INSTRUCTION_LENGTH 50
#define PIPELINE_STAGES 5
//計算機構成論week4
//fetch : 命令を取ってくる
//decode : レジスタファイルを読み出して命令をデコード
//register fetch :
//execute : 実行
//memory access :
//write back :

//IF -> ID -> EX -> MEM -> WB

int instruction_count = 0;

void execute_binary(char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int instruction_length, FILE* transition_file) {
    int current_line = 0;
    int total_cycles = 1;
    while (current_line < instruction_length) {    
        int pc = 0;
        pc = execute_binary_instruction(&binary_instructions[current_line].binary_code, 1, current_line);
        //register遷移の出力
        print_register_transition(transition_file, current_line);
        fflush(transition_file); 
        //printf("binary_insturcinos[current_line]:%s\n",binary_instructions[current_line].binary_code);
        printf("assembly_code:%20s",assembly_instructions[current_line+4]);
        // if(strcmp(binary_instructions[current_line].binary_code,"00000000000000000000000000000000") == 0){
        //     current_line++;
        //     printf("\n");
        //     continue;
        // } else {
        //     for (int i = 0; i < total_cycles; i++) {
        //         printf("     ");
        //     }
        //     printf(" IF  ");
        //     printf(" ID  ");
        //     printf(" EX  ");
        //     printf(" MEM ");
        //     printf(" WB  ");
        //     printf("\n");
        // }
        
        if (pc == 1) {
            current_line++;
        } else {
            current_line += pc;
        }
        total_cycles++;
    }
}

void print_pipeline_state(char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int current_line){
    printf("binary_code:%s\n",binary_instructions[current_line].binary_code);
    //binary codeに対応したassembly code
    printf("assembly_code: %-20s\n",assembly_instructions[current_line+4]);
    int total_cycles = current_line + PIPELINE_STAGES;
}

int main(){
    //ファイルからassembly取得
    FILE *file;
    char assembly_code[MAX_ASSEMBLY_SIZE];
    char assembly_instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH];

    file = fopen("assembly.txt","r");
    if(file == NULL){
        perror("Error opening file");
        return 1;
    }

    size_t read_size = fread(assembly_code, 1, sizeof(assembly_code)-1, file);
    if(read_size == 0){
        perror("Error reading file");
        fclose(file);
        return 1;
    }
    assembly_code[read_size] = '\0';
    fclose(file);
    //printf("assmbly_code:%s\n",assembly_code);

    //assmblyをbinaryに変換
    remove_comments(assembly_code);

    parse_assembly(assembly_code);

    char* token = strtok((char*)assembly_code, "\n");
    while (token != NULL && instruction_count < MAX_INSTRUCTIONS) {
        strncpy(assembly_instructions[instruction_count], token, MAX_INSTRUCTION_LENGTH - 1);
        assembly_instructions[instruction_count][MAX_INSTRUCTION_LENGTH - 1] = '\0'; // null終端を保証
        instruction_count++;
        token = strtok(NULL, "\n");
    }

    //binary codeはbinary.txtにoutput
    FILE *output_file = fopen("binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int instruction_length = print_binary_instructions(output_file);
    fclose(output_file);

    //register遷移
    FILE *transition_file = fopen("transition.md", "w");
    if (transition_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    for_markdown(transition_file);

    //pipeline
    //binary codeを受け取ってpipelineにする
    
    execute_binary(assembly_instructions, binary_instructions, instruction_length, transition_file);
    
    fclose(transition_file);

    return 0;
}