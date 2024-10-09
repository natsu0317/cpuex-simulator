#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 100

typedef struct{
    char opcode[10];
    char operand1[10];
    char operand2[10];
    char operand3[10];
} Instruction;

//サポートする命令と対応するオペコードのリスト
Instruction instructions[MAX_LENGTH];
int instruction_count = 0; //命令数

const char* r_type_opcodes[] = {"and","or","add"};
const char* a_type_opcodes[] = {"mv"};
const char* f_type_opcodes[] = {"eq","ne","lt","gt"};
const char* j_type_opcodes[] = {"jal"};
const char* m_type_opcodes[] = {"sw","lw"};
#include <string.h>

// オペコードが指定されたタイプに含まれるか確認する
int is_opcode_type(const char* opcode, const char** type_opcodes) {
    const char** op = type_opcodes;
    while (*op != NULL) {
        if (strcmp(opcode, *op) == 0) {
            return 1; // オペコードが見つかった場合
        }
        op++;
    }
    return 0; // オペコードが見つからなかった場合
}

// R型のオペコードを確認
int is_r_type(const char* opcode) {
    return is_opcode_type(opcode, r_type_opcodes);
}

// A系のオペコードを確認
int is_a_type(const char* opcode) {
    return is_opcode_type(opcode, a_type_opcodes);
}

// F系のオペコードを確認
int is_f_type(const char* opcode) {
    return is_opcode_type(opcode, f_type_opcodes);
}

// J系のオペコードを確認
int is_j_type(const char* opcode) {
    return is_opcode_type(opcode, j_type_opcodes);
}

// M系のオペコードを確認
int is_m_type(const char* opcode) {
    return is_opcode_type(opcode, m_type_opcodes);
}


const char* get_opcode_binary(const char* opcode){
    //R型(演算)
    //subはめんどくさいのでいったん無し
    if (strcmp(opcode, "and") == 0) return "111";
    if (strcmp(opcode, "or") == 0) return "110";
    if (strcmp(opcode, "add") == 0) return "000";
    //A系(代入)
    if (strcmp(opcode, "mv") == 0) return "0011";
    //F系(分岐)
    if (strcmp(opcode, "eq") == 0) return "0011";
    if (strcmp(opcode, "ne") == 0) return "0011";
    if (strcmp(opcode, "lt") == 0) return "0011";
    if (strcmp(opcode, "gt") == 0) return "0011";
    //J型(ジャンプ)
    if (strcmp(opcode, "jal") == 0) return "0011";
    //M系(メモリ)
    if (strcmp(opcode, "sw") == 0) return "101011";
    if (strcmp(opcode, "lw") == 0) return "100011";
    return "0000";
}

typedef struct{
    char binary_code[32];
}BinaryInstruction;


BinaryInstruction binary_instructions[MAX_LENGTH];
int binary_instruction_count = 0;//命令数

//アセンブリコードをバイナリに変換して出力
void parse_assembly(const char* assembly_code){
    const char* delimiter = "\n";
    //オリジナルのassmbly_codeを変更しないため
    char* code_copy = strdup(assembly_code);//assembly_codeの複製文字列へのポインタ
    char* token = strtok(code_copy, delimiter);//delimiter(\n)で分離

    while(token != NULL){
        char opcode[16], operand1[16], operand2[16], operand3[16];
        sscanf(token, "%s %s %s %s", opcode, operand1, operand2, operand3); 
        BinaryInstruction inst;
        const char* opcode_bin = get_opcode_binary(opcode);
        const char* rd_bin = get_register_binary(operand1);
        const char* reg1_bin = get_register_binary(operand1);
        const char* reg2_bin = get_register_binary(operand2);

        // if(operand2[0] == 'R'){
        //     const char* reg2_bin = get_register_binary(operand2);
        // } else {
        //     char* imm_bin = get_immediate_binary(operand2);
        //     free(imm_bin);
        // }  

        // if(operand3[0] == 'R'){
        //     const char* reg3_bin = get_register_binary(operand2);
        // } else {
        //     char* imm_bin = get_immediate_binary(operand2);
        //     free(imm_bin);
        // }

        //snprintf(inst.binary_code, 32, "%s %s %s", opcode_bin, reg1_bin, imm_bin);

        if(is_r_type(opcode)){
            snprintf(inst.binary_code, 32, "0000000%s%s%s%s0110011", reg2_bin, reg1_bin, opcode_bin,rd_bin);
        }
        if(is_a_type(opcode)){
            snprintf(inst.binary_code, 32, "%s %s %s", opcode_bin, reg1_bin, imm_bin);
        }
        if(is_f_type(opcode)){
            snprintf(inst.binary_code, 32, "%s %s %s", opcode_bin, reg1_bin, imm_bin);
        }
        if(is_j_type(opcode)){
            snprintf(inst.binary_code, 32, "%s %s %s", opcode_bin, reg1_bin, imm_bin);
        }
        if(is_m_type(opcode)){
            snprintf(inst.binary_code, 32, "%s %s %s", opcode_bin, reg1_bin, imm_bin);
        }

        binary_instructions[binary_instruction_count++] = inst;
        //次の行
        token = strtok(NULL,delimiter);
    }
    free(code_copy);
}

void print_binary_instructions(){
    for(int i=0;i<binary_instruction_count;i++){
        printf("Binary: %s\n",binary_instructions[i].binary_code);
    }
}

int main() {
    const char* assembly_code =
        "MOV R1 100\n"
        "ADD R2 R3\n"
        "SUB R0 50\n";

    // アセンブリコードをパースしてバイナリに変換
    parse_assembly(assembly_code);

    // すべてのバイナリコードを出力
    print_binary_instructions();

    return 0;
}