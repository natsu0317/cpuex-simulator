#ifndef BITORESULT_H
#define BITORESULT_H

typedef struct{
    int pc;
    int opcode; //sw,lwなら1,分岐命令なら2,それ以外なら0
    int operand1;
    int operand2;
    int operand3;
}Pc_operand;
typedef struct{
    int r_type[5]; // R型命令: {"add", "sub", "and", "or", "xor"}
    int i_type[5]; // I型命令: {"addi", "andi", "ori", "xori", "slli"}
    int s_type[1]; // S型命令: {"sw"}
    int b_type[6]; // B型命令: {"beq", "bne", "blt", "bge", "bltu", "bgeu"}
    int u_type[1]; // U型命令: {"lui"}
    int aui_type[1]; // AUIPC命令: {"auipc"}
    int j_type[1]; // J型命令: {"jal"}
    int jalr_type[1]; // JALR命令: {"jalr"}
    int lw_type[1]; // LW命令: {"lw"}
    int f_type[4]; // 浮動小数点命令: {"fadd", "fsub", "fmul", "fdiv"}
    int c_type[2]; // CSR命令: {"csrr", "csrw"}
} InstructionCounter;
Pc_operand execute_binary_instruction(const char binary_instruction[][33], int num_instructions, int current_line, FILE* sld_file, FILE* sld_result_file, FILE* memory_file );
extern InstructionCounter counter;
int result_main();
void print_register_transition(FILE *transition_file, FILE *float_transition_file, int pc);
void for_markdown(FILE *transition_file);

#endif