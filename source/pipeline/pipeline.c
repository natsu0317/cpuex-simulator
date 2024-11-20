#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../asm_to_binary/asm_to_binary.h"
#include "../binary_to_result/binary_to_result.h"

#define MAX_ASSEMBLY_SIZE 4096  // アセンブリコードの最大サイズ
#define MAX_INSTRUCTION_LENGTH 100
#define PIPELINE_STAGES 5
#define INST_WIDTH 20
#define CYCLE_WIDTH 5
#define STAGE_WIDTH 5
//計算機構成論week4
//fetch : 命令を取ってくる
//decode : レジスタファイルを読み出して命令をデコード
//register fetch :
//execute : 実行
//memory access :
//write back :

//IF -> ID -> EX -> MEM -> WB
void clean_instruction(char *dest, const char *src, size_t width) {
    size_t i = 0, j = 0;
    // 先頭の空白をスキップ
    while (isspace((unsigned char)src[i])) i++;
    // タブを空白に置換しながらコピー
    for (; src[i] != '\0' && j < width - 1; i++) {
        if (src[i] == '\t') {
            dest[j++] = ' ';
        } else {
            dest[j++] = src[i];
        }
    }
    // 残りを空白で埋める
    while (j < width - 1) {
        dest[j++] = ' ';
    }
    dest[j] = '\0';
}

void print_pipeline(FILE* pipeline_file, char assembly_instructions[][MAX_INSTRUCTION_LENGTH], int current_line, int assembly_count, int total_cycles){
    char formatted_instruction[INST_WIDTH + 1];  

    for (int i = 0; i < total_cycles; i++) {
        fprintf(pipeline_file, "%*s", CYCLE_WIDTH, "");
    }
    
    clean_instruction(formatted_instruction, assembly_instructions[current_line+assembly_count], INST_WIDTH + 1);

    fprintf(pipeline_file, "%s", formatted_instruction);

    fprintf(pipeline_file, "%*s", STAGE_WIDTH, "IF");
    fprintf(pipeline_file, "%*s", STAGE_WIDTH, "ID");
    fprintf(pipeline_file, "%*s", STAGE_WIDTH, "EX");
    fprintf(pipeline_file, "%*s", STAGE_WIDTH, "MEM");
    fprintf(pipeline_file, "%*s", STAGE_WIDTH, "WB");
    fprintf(pipeline_file, "\n");
}

void print_stall(FILE* pipeline_file, int stall_cycle, int total_cycle){
    for(int j=0; j<stall_cycle; j++){
        for (int i = 0; i < total_cycle; i++) {
            fprintf(pipeline_file, "%*s", CYCLE_WIDTH, "");
        }
        while (j < INST_WIDTH + 1) {
            fprintf(pipeline_file, " ");
            j++;
        }

        fprintf(pipeline_file, "%*s", STAGE_WIDTH, "     ");
        fprintf(pipeline_file, "%*s", STAGE_WIDTH, "     ");
        fprintf(pipeline_file, "%*s", STAGE_WIDTH, "STALL");
        fprintf(pipeline_file, "%*s", STAGE_WIDTH, "     ");
        fprintf(pipeline_file, "%*s", STAGE_WIDTH, "     ");
        fprintf(pipeline_file, "\n");
        total_cycle++;
        }
}

// 行数 - 1
int instruction_count = 0;

void execute_binary(int assembly_count, char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int instruction_length, FILE* transition_file, FILE* pipeline_file) {
    int current_line = 1; // assembly codeの行数に対応
    int total_cycles = 1;
    while (current_line < instruction_length) {   
        printf("pipeline start\n");
        printf("行数：%d\n",current_line); 
        printf("binary:%s\n",binary_instructions[current_line - 1].binary_code);
        //finish命令
        if(strcmp(binary_instructions[current_line - 1].binary_code,"11111111111111111111111111111111") == 0){
            break;
        }
        int pc = 0;
        Pc_operand pc_opcode_operand1;
        pc_opcode_operand1 = execute_binary_instruction(&binary_instructions[current_line - 1].binary_code, 1, current_line - 1);
        pc = pc_opcode_operand1.pc;
        int opcode = pc_opcode_operand1.opcode;//1 = sw / lw, 2 = 分岐命令
        int operand2 = pc_opcode_operand1.operand2;
        int operand3 = pc_opcode_operand1.operand3;
        // printf("pc:%d\n",pc);
        // printf("operand1:%d\n",pc_opcode_operand1.operand1);
        // printf("opcode:%d\n",pc_opcode_operand1.opcode);//1 = sw / lw, 2 = 分岐命令

        int save_operand[2];
        save_operand[0] = save_operand[1]; // 1個前のoperand1の値
        save_operand[1] = pc_opcode_operand1.operand1;// 現在のoperand1の値

        //register遷移の出力
        print_register_transition(transition_file, current_line);
        fflush(transition_file); 
        printf("binary_insturcinos[current_line]:%s\n",binary_instructions[current_line - 1].binary_code);
        printf("assembly_code:%20s\n",assembly_instructions[current_line+assembly_count]);
        if(strcmp(binary_instructions[current_line - 1].binary_code,"00000000000000000000000000000000") == 0){
            current_line++;
            printf("\n");
            continue;
        } else {      
            //pipeline 出力

            print_pipeline(pipeline_file, assembly_instructions, current_line - 1, assembly_count, total_cycles);

            //data hazard
            //現在のoperand2 or operand3 == n個前のoperand1 -> (4-n)サイクルstall
            // -> フォワーディングにより解決
            //sw, lwが来たらその後1クロックstall

            //フォワーディングがない場合
            // for(int i=2;i>=0;i--){
            //     if(operand2 == save_operand[i]){
            //         print_stall(pipeline_file, 4-i, total_cycles);
            //         total_cycles += (4-i);
            //         break;
            //     } else if (operand3 == save_operand[i]){
            //         print_stall(pipeline_file, 4-i, total_cycles);
            //         total_cycles += (4-i);
            //         break;
            //     }
            // }
            if( opcode == 1){
                if(operand2 == save_operand[0] || operand3 == save_operand[0]){
                    print_stall(pipeline_file, 1, total_cycles);
                    total_cycles++;
                }
            }
            //制御 hazard
            //分岐命令の後は1サイクルストール
            if( opcode == 2 ){
                print_stall(pipeline_file, 1, total_cycles);
                total_cycles++;
            }
        }
        printf("pc:%d\n",pc);
        
        if (pc == 1) {
            current_line++;
        } else {
            current_line += pc;
        }
        printf("current_line:159 %d\n",current_line);
        total_cycles++;
    }
}

void print_pipeline_state(char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int current_line){
    printf("binary_code:%s\n",binary_instructions[current_line].binary_code);
    //binary codeに対応したassembly code
    printf("assembly_code: %-20s\n",assembly_instructions[current_line]);
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
    printf("assmbly_code:%s\n",assembly_code);

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
    //printf("count:%d\n",instruction_count);

    //binary codeはbinary.txtにoutput
    FILE *output_file = fopen("binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int instruction_length = print_binary_instructions(output_file) + 1;
    //printf("%d\n",instruction_length);
    fclose(output_file);
    printf("length:%d\n",instruction_length);
    int assembly_count =  instruction_count - instruction_length;
    //printf("assembly_code:%20s",assembly_instructions[assembly_count]);

    //register遷移
    FILE *transition_file = fopen("transition.md", "w");
    if (transition_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    for_markdown(transition_file);

    //pipeline
    //binary codeを受け取ってpipelineにする
    FILE *pipeline_file = fopen("pipeline.txt","w");
    if (pipeline_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }
    
    execute_binary(assembly_count, assembly_instructions, binary_instructions, instruction_length, transition_file, pipeline_file);
    
    fclose(transition_file);

    return 0;
}