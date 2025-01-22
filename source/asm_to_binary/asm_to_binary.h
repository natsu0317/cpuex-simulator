#ifndef ASMTOBI_H
#define ASMTOBI_H

#define MAX_INSTRUCTIONS 50000
#define INSTRUCTION_LENGTH 33  // 32ビット + null終端文字
#define MAX_LABELS 1000

typedef struct {
    char binary_code[INSTRUCTION_LENGTH];
} BinaryInstruction;

extern BinaryInstruction binary_instructions[MAX_INSTRUCTIONS];

extern int instruction_count;
extern double float_memory[];
extern int label_count;

typedef struct {
    char label[256];
    int line_number;
} LabelEntry;

void remove_comments(char *code);
LabelEntry found_labels(const char* assembly_code);
void parse_assembly(const char* assembly_code);
int print_binary_instructions(FILE* output_file);
extern int use_register[64];

#endif