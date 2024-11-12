#ifndef ASMTOBI_H
#define ASMTOBI_H

void remove_comments(char *code);
void parse_assembly(const char* assembly_code);
void print_binary_instructions(FILE* output_file);

#endif