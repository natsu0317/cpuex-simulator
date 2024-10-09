#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 100

int instruction_count = 0;

typedef struct{
    char binary_code[32];
} BinaryInstruction;

BinaryInstruction binary_instructions[MAX_LENGTH];
int binary_instruction_count = 0;

const char* r_type_opcodes[] = {"add","sub","and","or","xor"};
const char* i_type_opcodes[] = {"addi","andi","ori","xori"};
const char* b_type_opcodes[] = {"beq","bne","blt","bge","bltu","bgeu"};
const char* j_type_opcodes[] = {"jal",};

int is_opcode_type(const char* opcode,const char** type_opcodes){
    const char** op = type_opcodes;
    while(*op != NULL){
        if(strcmp(opcode,*op) == 0){
            return 1;
        }
        op++;
    }
    return 0;
}

int is_r_type(const char* opcode){
    return is_opcode_type(opcode,r_type_opcodes);
}

int is_i_type(const char* opcode){
    return is_opcode_type(opcode,i_type_opcodes);
}

int is_b_type(const char* opcode){
    return is_opcode_type(opcode,b_type_opcodes);
}

int is_j_type(const char* opcode){
    return is_opcode_type(opcode,j_type_opcodes);
}

const char* get_opcode_binary(const char* opcode){
    //R : add
    //funct7 | rs2 | rs1 | funct3 | rd | opcode
    if(strcmp(opcode,"add") == 0 || strcmp(opcode,"sub") == 0) return "0110011";
    if(strcmp(opcode,"and") == 0 || strcmp(opcode, "or") == 0 || strcmp(opcode, "xor") == 0)  return "0110011";
    //I : addi
    //imm[11:0] | rs1 | funct3 | r | opcode
    //funct3に対応
    if(strcmp(opcode,"addi") == 0) return "000";
    if(strcmp(opcode,"andi") == 0) return "111";
    if(strcmp(opcode,"ori") == 0) return "110";
    if(strcmp(opcode,"xori") == 0) return "100";

    //S
    //imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode

    //B : bne
    //funct3に対応
    //imm[12|10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
    if(strcmp(opcode,"beq") == 0) return "000";
    if(strcmp(opcode,"bne") == 0) return "001";
    if(strcmp(opcode,"blt") == 0) return "100";
    if(strcmp(opcode,"bge") == 0) return "101";
    if(strcmp(opcode,"bltu") == 0) return "110";
    if(strcmp(opcode,"bgeu") == 0) return "111";

    //U
    //imm[31:12] | rd | opcode

    //J : JAL
    //imm[20|10:1|11|19:12] | rd | opcode
    if(strcmp(opcode,"jal") == 0) return "1101111";

}

const char* get_register_binary(const char* reg){
    if (strcmp(reg, "R0") == 0) return "00000";
    if (strcmp(reg, "R1") == 0) return "00001";
    if (strcmp(reg, "R2") == 0) return "00010";
    if (strcmp(reg, "R3") == 0) return "00011";
    return "000";  // デフォルト
}

// 即値をバイナリに変換
const char* get_immediate_binary(const char* imm) {
    static char binary[16]; // 結果を保持するバッファ
    int imm_value = atoi(imm); // 即値を整数に変換
    // バッファをクリア
    memset(binary, 0, sizeof(binary));
    // 即値を2進数に変換
    for (int i = 11; i >= 0; i--) { // 12ビット分の即値を格納
        binary[11 - i] = (imm_value & (1 << i)) ? '1' : '0';
    }
    return binary;
}


//文字列をスライス
//[a:b]の時はstart = strle(source)-a, length = b-a+1
void get_substring(const char* source, char* destination, int start, int length){
    strncpy(destination, source+start, length);
    destination[length] = '\0';
}

void parse_assembly(const char* assembly_code){
    const char* delimiter = "\n";
    char* code_copy = strdup(assembly_code);
    char* token = strtok(code_copy, delimiter);
    printf("%s\n",token);

    while (token != NULL){
        char opcode[16],operand1[32],operand2[32],operand3[32];
        sscanf(token, "%s %s %s %s", opcode, operand1, operand2, operand3); 
        printf("Opcode: %s, Operand1: %s, Operand2: %s, Operand3: %s\n", opcode, operand1, operand2, operand3);

        const char* opcode_bin = get_opcode_binary(opcode);
        const char* rd_bin = get_register_binary(operand1);
        const char* r1_bin = operand2[0] == 'R' ? get_register_binary(operand2) : get_immediate_binary(operand2);
        const char* r2_bin = operand3[0] == 'R' ? get_register_binary(operand3) : get_immediate_binary(operand3);

        BinaryInstruction inst;

        if(is_r_type(opcode)){
            if(strcmp(opcode, "add") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "sub") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0100000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "and") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s111%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "or") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s110%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "xor") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s100%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
        }

        if(is_i_type(opcode)){
            //[11:0]
            char r2_bin_sub[13];//12bit + 終端文字
            get_substring(r2_bin,r2_bin_sub,strlen(r2_bin)-12,12);
            
            printf("opcode_bin: %s, rd: %s, r1: %s, r2_bin: %s, r2_bin_sub: %s\n", opcode_bin, rd_bin, r1_bin, r2_bin, r2_bin_sub);
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s%s0010011",r2_bin_sub,r1_bin,opcode_bin,rd_bin);
        }

        if(is_b_type(opcode)){
            //opcode -> funct3, rd -> r1, r1 -> r2, r2 -> offsetに対応
            char imm_1[8],imm_2[6];
            char bit12[2],bit10_5[7],bit4_1[5],bit11[2];

            get_substring(r2_bin,bit12,strlen(r2_bin)-12,1);
            get_substring(r2_bin,bit10_5,strlen(r2_bin)-10,6);
            snprintf(imm_1, 8, "%s%s", bit12, bit10_5); // 12ビットと10:5ビットを結合

            get_substring(r2_bin,bit4_1,strlen(r2_bin)-4,4);
            get_substring(r2_bin,bit11,strlen(r2_bin)-11,1);
            snprintf(imm_2, 6, "%s%s", bit4_1, bit11); // 4:1ビットと11ビットを結合

            printf("imm_1: %s, size: %lu\n", imm_1, strlen(imm_1));
            printf("imm_2: %s, size: %lu\n", imm_2, strlen(imm_2));
            printf("r1_bin: %s, size: %lu\n", r1_bin, strlen(r1_bin));
            printf("rd_bin: %s, size: %lu\n", rd_bin, strlen(rd_bin));
            printf("opcode: %s, size: %lu\n", opcode, strlen(opcode));

            //snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s%s%s1100011",imm_1,r1_bin,rd_bin,opcode,imm_2);
        }

        if(is_j_type(opcode)){
            //r1 -> offsetに対応
            char bit20[2],bit10_1[11],bit11[2],bit19_12[9];
            char imm[21];
            
            get_substring(r1_bin,bit20,strlen(r1_bin)-20,1);
            get_substring(r1_bin,bit10_1,strlen(r1_bin)-10,10);
            get_substring(r1_bin,bit11,strlen(r1_bin)-11,1);
            get_substring(r1_bin,bit19_12,strlen(r1_bin)-19,8);
            snprintf(imm, 21,"%s%s%s%s", bit20, bit10_1, bit11, bit19_12);
            
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s",imm, rd_bin, opcode_bin);
        }

        printf("Binary Code: %s\n",inst.binary_code);

        binary_instructions[binary_instruction_count++] = inst;

        token = strtok(NULL,delimiter);

    }
    free(code_copy);
}

// すべてのバイナリ命令を出力
void print_binary_instructions() {
    for (int i = 0; i < binary_instruction_count; i++) {
        printf("Binary: %s\n", binary_instructions[i].binary_code);
    }
}

int main(){
    printf("hello\n");
    const char* assembly_code = 
        "add R1 R2 R3\n"
        "addi R1 R2 100\n"
        //beqとjalがうまくいってなさそう
        //多分offsetの問題
        "beq R1 R2 R3\n"
        "jal R1 R2\n";

    parse_assembly(assembly_code);

    print_binary_instructions();
    return 0;
}