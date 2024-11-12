#ifndef BITORESULT_H
#define BITORESULT_H


int execute_binary_instruction(const char binary_instruction[][33], int num_instructions, int current_line);
int result_main();
void print_register_transition(FILE *transition_file, int pc);
void for_markdown(FILE *transition_file);

#endif