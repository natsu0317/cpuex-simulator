#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "../asm_to_binary/asm_to_binary.h"
#include "../binary_to_result/binary_to_result.h"

#define MAX_ASSEMBLY_SIZE 2096896  // アセンブリコードの最大サイズ
#define MAX_INSTRUCTION_LENGTH 70 // 1行の長さ
#define PIPELINE_STAGES 5
#define INST_WIDTH 20
#define CYCLE_WIDTH 5
#define STAGE_WIDTH 5

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
void execute_binary(int assembly_count, char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int instruction_length, FILE* transition_file, FILE* float_transition_file, FILE* pipeline_file, FILE* sld_file, FILE* sld_result_file, FILE* memory_file, int use_register[64]) {
    int current_line = 1; // assembly codeの行数に対応
    int total_cycles = 1;
    int break_count = 1;
    const int MAX_BREAKS = 753; //(MAX_BREAKS+1回目のbreakで停止)
    bool step_mode = false;
    const int STOP_AT_INSTRUCTION = 353849; // この行数で停止

    while (current_line < instruction_length) {
        printf("\n\nnew instruction start\n");
        printf("binary_code:%s\n",binary_instructions[current_line - 1].binary_code);
        printf("%d行\n",current_line);
        if(total_cycles >= STOP_AT_INSTRUCTION) {
            printf("Type 'c' to continue, 's' for step-by-step execution, or 'q' to quit.\n");
            step_mode = true;
        }

        if (step_mode || (break_count > MAX_BREAKS)) {
            printf("Type 'c' to continue to next break, 's' for step-by-step execution, or 'q' to quit.\n");
            char command[256];
            if (fgets(command, sizeof(command), stdin) != NULL) {
                command[strcspn(command, "\n")] = 0;  // 改行をヌル文字に置き換える
                if (strcmp(command, "c") == 0) {
                    step_mode = false;
                    // 次のbreakまで実行を続ける
                } else if (strcmp(command, "s") == 0) {
                    step_mode = true;
                    // 1命令だけ実行して再度停止
                } else if (strcmp(command, "q") == 0) {
                    printf("Execution terminated by user.\n");
                    return;  // プログラムを終了
                } else {
                    printf("Unknown command. Please type 'c' to continue, 's' for step-by-step, or 'q' to quit.\n");
                    continue;  // コマンド入力に戻る
                }
            } else {
                fprintf(stderr, "Error reading input\n");
                return;  // エラーが発生したら関数を終了
            }
        }

        //finish命令
        if(strcmp(binary_instructions[current_line - 1].binary_code, "11111111111111111111111111111111") == 0){
            printf("Finish instruction detected.\n");
            break;
        }
        //break命令
        if(strcmp(binary_instructions[current_line - 1].binary_code, "11111111111111111111111111111110") == 0){
            break_count++;
            printf("Break %d at line %d.\n", break_count, current_line);
            
            if (break_count > MAX_BREAKS) {
                printf("Maximum number of breaks (%d) reached. Execution paused.\n", MAX_BREAKS);
                step_mode = true;
                continue;  // ループの先頭に戻ってユーザー入力を待つ
            }
        }
        int pc = 0;
        Pc_operand pc_opcode_operand1;
        // 2個前の命令を記録するため
        int two_previous = 0;
        // printf("previous:%s\n",binary_instructions[current_line - 2].binary_code);
        if(current_line > 2){
            if(strcmp(binary_instructions[current_line - 3].binary_code + 28, "0110") == 0){ //6
                two_previous = 1; //2つ前の命令がauipc(laの分岐用)
            }
            if(strcmp(binary_instructions[current_line - 3].binary_code + 28, "0101") == 0){ //5
                two_previous = 2; //2つ前の命令がlui
            }
        }
        pc_opcode_operand1 = execute_binary_instruction(&binary_instructions[current_line - 1].binary_code, two_previous, 1, current_line - 1, sld_file, sld_result_file, memory_file);
        pc = pc_opcode_operand1.pc;
        int opcode = pc_opcode_operand1.opcode;//1 = sw / lw, 2 = 分岐命令
        int operand2 = pc_opcode_operand1.operand2;
        int operand3 = pc_opcode_operand1.operand3;
        // printf("pipeline_pc:%d\n",pc);
        // printf("operand1:%d\n",pc_opcode_operand1.operand1);
        // printf("opcode:%d\n",pc_opcode_operand1.opcode);//1 = sw / lw, 2 = 分岐命令

        int save_operand[2];
        save_operand[0] = save_operand[1]; // 1個前のoperand1の値
        save_operand[1] = pc_opcode_operand1.operand1;// 現在のoperand1の値

        //register遷移の出力
        print_use_register_transition(transition_file, current_line, use_register);
        print_use_float_register_transition(float_transition_file, current_line, use_register);
        fflush(transition_file); 


        // printf("binary_insturcinos[current_line]:%s\n",binary_instructions[current_line - 1].binary_code);
        // printf("assembly_code:%20s\n",assembly_instructions[current_line+assembly_count]);
        // if(strcmp(binary_instructions[current_line - 1].binary_code,"00000000000000000000000000000000") == 0){
        //     current_line++;
        //     printf("\n");
        //     continue;
        // } else {      
        //     //pipeline 出力

        //     print_pipeline(pipeline_file, assembly_instructions, current_line - 1, assembly_count, total_cycles);

        //     //data hazard
        //     //現在のoperand2 or operand3 == n個前のoperand1 -> (4-n)サイクルstall
        //     // -> フォワーディングにより解決
        //     //sw, lwが来たらその後1クロックstall

        //     //フォワーディングがない場合
        //     // for(int i=2;i>=0;i--){
        //     //     if(operand2 == save_operand[i]){
        //     //         print_stall(pipeline_file, 4-i, total_cycles);
        //     //         total_cycles += (4-i);
        //     //         break;
        //     //     } else if (operand3 == save_operand[i]){
        //     //         print_stall(pipeline_file, 4-i, total_cycles);
        //     //         total_cycles += (4-i);
        //     //         break;
        //     //     }
        //     // }
        //     if( opcode == 1){
        //         if(operand2 == save_operand[0] || operand3 == save_operand[0]){
        //             print_stall(pipeline_file, 1, total_cycles);
        //             total_cycles++;
        //         }
        //     }
        //     //制御 hazard
        //     //分岐命令の後は1サイクルストール
        //     if( opcode == 2 ){
        //         print_stall(pipeline_file, 1, total_cycles);
        //         total_cycles++;
        //     }
        // }
        // printf("pc:%d\n",pc);
        
        if (pc == 1) {
            current_line++;
        } else {
            current_line += pc;
        }
        // printf("current_line: %d\n",current_line);
        total_cycles++;
    }
    printf("合計命令数: %d\n",total_cycles);
}

void print_pipeline_state(char assembly_instructions[][MAX_INSTRUCTION_LENGTH], BinaryInstruction binary_instructions[], int current_line){
    printf("binary_code:%s\n",binary_instructions[current_line].binary_code);
    //binary codeに対応したassembly code
    printf("assembly_code: %-20s\n",assembly_instructions[current_line]);
    int total_cycles = current_line + PIPELINE_STAGES;
}

InstructionCounter counter = {0};

void print_instruction_count(FILE* instruction_statics_file) {
    fprintf( instruction_statics_file, "\n=== Instruction Counts ===\n");

    // R型命令のカウント
    fprintf( instruction_statics_file, "\nR-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "add: %d, sub: %d, and: %d, xor: %d, div10: %d, sll: %d, srl: %d, sra: %d\n",
           counter.r_type[0], counter.r_type[1], counter.r_type[2], counter.r_type[4], counter.r_type[5], counter.r_type[6], counter.r_type[7], counter.r_type[8]);

    // I型命令のカウント
    fprintf( instruction_statics_file, "\nI-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "addi: %d, andi: %d, ori: %d, xori: %d, slli: %d\n",
           counter.i_type[0], counter.i_type[1], counter.i_type[2], counter.i_type[3], counter.i_type[4]);

    // S型命令のカウント
    fprintf( instruction_statics_file, "\nS-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "sw: %d\n", counter.s_type[0]);

    // B型命令のカウント
    fprintf( instruction_statics_file, "\nB-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "beq: %d, bne: %d, blt: %d, bge: %d, bgt: %d\n",
           counter.b_type[0], counter.b_type[1], counter.b_type[2],
           counter.b_type[3], counter.b_type[4]);

    // U型命令のカウント
    fprintf( instruction_statics_file, "\nU-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "lui: %d\n", counter.u_type[0]);

    // AUIPC命令のカウント
    fprintf( instruction_statics_file, "\nAUIPC Instruction Counts:\n");
    fprintf( instruction_statics_file, "auipc: %d\n", counter.aui_type[0]);

    // J型命令のカウント
    fprintf( instruction_statics_file, "\nJ-type Instruction Counts:\n");
    fprintf( instruction_statics_file, "jal: %d\n", counter.j_type[0]);

    // JALR命令のカウント
    fprintf( instruction_statics_file, "\nJALR Instruction Counts:\n");
    fprintf( instruction_statics_file, "jalr: %d\n", counter.jalr_type[0]);

    // LW命令のカウント
    fprintf( instruction_statics_file, "\nLW Instruction Counts:\n");
    fprintf( instruction_statics_file, "lw: %d\n", counter.lw_type[0]);

    // 浮動小数点命令のカウント
    fprintf( instruction_statics_file, "\nFloating-point Instruction Counts:\n");
    fprintf( instruction_statics_file, "fadd: %d, fsub: %d, fmul: %d, fdiv: %d, fabs: %d, fneg: %d, finv: %d, fsqrt: %d, fsgnjn: %d, fsgnjx: %d, feq: %d, flt: %d, fcvtws: %d, fcvtsw: %d, floor: %d\n",
           counter.f_type[0], counter.f_type[1], counter.f_type[2], counter.f_type[3], counter.f_type[4], counter.f_type[5], counter.f_type[6], counter.f_type[7], counter.f_type[8], counter.f_type[9], counter.f_type[10], counter.f_type[11], counter.f_type[12], counter.f_type[13], counter.f_type[14]);

    // CSR命令のカウント
    fprintf( instruction_statics_file, "\nCSR Instruction Counts:\n");
    fprintf( instruction_statics_file, "csrr: %d, csrw: %d\n", counter.c_type[0], counter.c_type[1]);

    fprintf( instruction_statics_file, "\n===========================\n");
}

int main(){
    clock_t start_time, end_time;
    start_time = clock();

    //ファイルからassembly取得
    FILE *file;
    char assembly_code[MAX_ASSEMBLY_SIZE];
    char assembly_instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH];
    file = fopen("./document/assembly.txt","r");
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
    // printf("assmbly_code:%s\n",assembly_code);

    //assmblyをbinaryに変換
    remove_comments(assembly_code);

    // labelとその位置を記録
    found_labels(assembly_code);
    parse_assembly(assembly_code);

    char* token = strtok((char*)assembly_code, "\n");
    while (token != NULL && instruction_count < MAX_INSTRUCTIONS) {
        strncpy(assembly_instructions[instruction_count], token, MAX_INSTRUCTION_LENGTH - 1);
        assembly_instructions[instruction_count][MAX_INSTRUCTION_LENGTH - 1] = '\0'; // null終端を保証
        instruction_count++;
        token = strtok(NULL, "\n");
    }
    // printf("count:%d\n",instruction_count);

    //binary codeはbinary.txtにoutput
    FILE *output_file = fopen("./document/binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int instruction_length = print_binary_instructions(output_file) + 1;
    //printf("%d\n",instruction_length);
    fclose(output_file);
    // printf("length:%d\n",instruction_length);
    int assembly_count =  instruction_count - instruction_length;
    //printf("assembly_code:%20s",assembly_instructions[assembly_count]);

    //register遷移
    FILE *transition_file = fopen("./document/transition.md", "w");
    if (transition_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    // for_markdown(transition_file, use_register);

    FILE *float_transition_file = fopen("./document/float_transition.md", "w");
    if (float_transition_file == NULL) {
        perror("Error opening float_transition file");
        return 1;
    }

    for_markdown(transition_file, float_transition_file, use_register);

    //pipeline
    //binary codeを受け取ってpipelineにする
    FILE *pipeline_file = fopen("./document/pipeline.txt","w");
    if (pipeline_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    FILE *sld_file = fopen("./document/formatted_sld_data.txt","r");
    if (sld_file == NULL) {
        perror("Error opening sld file");
        return 1;
    }

    //memoryの値の遷移を表示
    FILE *memory_file = fopen("./document/memory_transition.txt","w");
    if (memory_file == NULL) {
        perror("Error opening memory file");
        return 1;
    }

    //x10の値が格納
    FILE *sld_result_file = fopen("./document/sld_result.txt","w");
    if (sld_result_file == NULL) {
        perror("Error opening sld_result file");
        return 1;
    }
    
    execute_binary(assembly_count, assembly_instructions, binary_instructions, instruction_length, transition_file, float_transition_file, pipeline_file, sld_file, sld_result_file, memory_file, use_register);
    
    FILE *instruction_statics_file = fopen("./document/instruction_statics.txt","w");
    if (instruction_statics_file == NULL) {
        perror("Error opening sld_result file");
        return 1;
    }

    print_instruction_count(instruction_statics_file);

    fclose(transition_file);
    fclose(sld_file);

    end_time = clock();

    printf("Execution Time = %lf [s]\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}