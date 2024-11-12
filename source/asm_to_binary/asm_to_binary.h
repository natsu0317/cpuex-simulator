#ifndef ASMTOBI_H
#define ASMTOBI_H

#define MAX_INSTRUCTIONS 1000
#define INSTRUCTION_LENGTH 33  // 32ビット + null終端文字

typedef struct {
    char binary_code[INSTRUCTION_LENGTH];
} BinaryInstruction;

extern BinaryInstruction binary_instructions[MAX_INSTRUCTIONS];

extern int instruction_count;
void remove_comments(char *code);
void parse_assembly(const char* assembly_code);
int print_binary_instructions(FILE* output_file);

#endif