#ifndef BITORESULT_H
#define BITORESULT_H

typedef struct{
    int pc;
    int opcode; //sw,lwなら1,分岐命令なら2,それ以外なら0
    int operand1;
    int operand2;
    int operand3;
}Pc_operand;

Pc_operand execute_binary_instruction(const char binary_instruction[][33], int num_instructions, int current_line, FILE* sld_file, FILE* sld_result_file );
int result_main();
void print_register_transition(FILE *transition_file, int pc);
void for_markdown(FILE *transition_file);

#endif