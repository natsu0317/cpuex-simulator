#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
extern int instruction_count;

//アセンブリコードをバイナリコードにparse

#define MAX_INSTRUCTIONS 50000
#define MAX_ASSEMBLY_SIZE 2096896  // アセンブリコードの最大サイズ
#define MAX_LABELS 1000 // ラベルの最大個数

typedef struct{
    char binary_code[33];
} BinaryInstruction;

BinaryInstruction binary_instructions[MAX_INSTRUCTIONS];
int binary_instruction_count = 0;
double float_memory[256]; //メモリに浮動小数点の値を格納

// mv li ret j
// mv a b = add a, x0, b(a = b)
// li a imm = addi a, x0, imm(a = imm)
// ret = jalr x0, x1, 0
// bgt a b = blt b a

const char* r_type_opcodes[] = {"add", "sub", "and", "xor", "div10", "rem", "sll", "sra", "srl", NULL};
const char* i_type_opcodes[] = {"addi", "uaddi", "andi", "ori", "xori", "slli", NULL};
const char* s_type_opcodes[] = {"sw", NULL};
const char* b_type_opcodes[] = {"beq", "bne", "blt", "bge", "bltu", "bgeu", NULL}; 
const char* u_type_opcodes[] = {"lui", NULL};
const char* aui_type_opcodes[] = {"auipc", NULL};
const char* j_type_opcodes[] = {"jal", NULL};
const char* jalr_type_opcodes[] = {"jalr", NULL};
const char* lw_type_opcodes[] = {"lw", NULL};
const char* f_type_opcodes[] = {"fadd", "fsub", "fmul", "fdiv", "fabs", "fneg", "finv", "fsqrt", "fsgnjn", "fsgnjx", "feq", "flt", "fcvtws", "fcvtsw", "floor", NULL};
const char* c_type_opcodes[] = {"csrr", "csrw", "csrw_int", NULL};

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
int is_s_type(const char* opcode){
    return is_opcode_type(opcode,s_type_opcodes);
}
int is_b_type(const char* opcode){
    return is_opcode_type(opcode,b_type_opcodes);
}
int is_u_type(const char* opcode){
    return is_opcode_type(opcode,u_type_opcodes);
}
int is_aui_type(const char* opcode){
    return is_opcode_type(opcode,aui_type_opcodes);
}
int is_j_type(const char* opcode){
    return is_opcode_type(opcode,j_type_opcodes);
}
int is_jalr_type(const char* opcode){
    return is_opcode_type(opcode,jalr_type_opcodes);
}
int is_lw_type(const char* opcode){
    return is_opcode_type(opcode,lw_type_opcodes);
}
int is_f_type(const char* opcode){
    return is_opcode_type(opcode,f_type_opcodes);
}
int is_c_type(const char* opcode){
    return is_opcode_type(opcode,c_type_opcodes);
}

const char* get_opcode_binary(const char* opcode){
    //R : add
    //funct7 | rs2 | rs1 | funct3 | rd | opcode
    if(is_r_type(opcode)) return "0001";
    //I : addi
    //imm[11:0] | rs1 | funct3 | r | opcode
    //funct3に対応
    if(is_i_type(opcode)) return "0010";
    //S
    //imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode
    if(is_s_type(opcode)) return "0011";
    //B : bne
    //funct3に対応
    //imm[12|10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
    if(is_b_type(opcode)) return "0100";
    //U
    //imm[31:12] | rd | opcode
    if(is_u_type(opcode)) return "0101";
    //AUIPC
    if(is_aui_type(opcode)) return "0110";
    //J : JAL
    //imm[20|10:1|11|19:12] | rd | opcode
    if(is_j_type(opcode)) return "0111";
    //JALR: jalr
    if(is_jalr_type(opcode)) return "1000";
    //lw
    if(is_lw_type(opcode)) return "1001";
    //F
    // opcode | rs2 | rs1 | rm | rd | 1010011
    if(is_f_type(opcode)) return "1010";
    //c
    if(is_c_type(opcode)) return "1011";
    //FINISH
    if(strcmp(opcode,"finish") == 0) return "1111111";
}

char* get_register_binary(const char* reg) {
    char* binary = (char*)malloc(7*sizeof(char));
    if(binary == NULL){
        return NULL;
    } 
    int reg_num;
    if (sscanf(reg, "x%d", &reg_num) != 1 || reg_num < 0 || reg_num > 63) {
        free(binary);
        return "000000"; 
    }
    // 2進数に変換する
    for (int i = 5; i >= 0; --i) {
        binary[i] = (reg_num % 2) + '0';
        reg_num /= 2;
    }
    binary[6] = '\0'; // 終端文字を追加
    return binary;
}

// intをbinaryに
char* int_to_binary(int value){
    ////printf("value: %d\n",value);
    char* binary = (char*)malloc(33 * sizeof(char));
    if (binary == NULL){
        return NULL;
    }
    memset(binary, 0, 33);
    for(int i=31; i>=0; i--){
        binary[31-i] = (value & (1 << i)) ? '1' : '0';
    }
    binary[32] = '\0';
    return binary;
}

// floatをbinaryに
char* float_to_binary(float value){
    // inputとoutputは同じメモリ領域を共有
    union{
        float input;
        int output;
    } data;
    data.input = value;
    return int_to_binary(data.output);
}

// 文字列がfloatかどうかを判断
bool is_float(const char* str){
    while(*str){
        if(*str == '.'){
            return true;
        }
        str++;
    }
    return false;
}

// 即値をバイナリに変換
char* get_immediate_binary(const char* imm) {
    // //printf("get_immediate_binary\n");
    
    // "0x"で始まるかどうかを確認
    if (strncmp(imm, "0x", 2) == 0) {
        //printf("imm: %s\n", imm);
        unsigned int number = (unsigned int)strtol(imm, NULL, 16);

        // 最上位2ビットをクリアして"01"に設定
        // number &= ~(3 << 30); // 最上位2ビットをクリア
        // number |= (1 << 30);  // 最上位2ビットを"01"に設定

        // バイナリ文字列への変換
        char *binary_str = malloc(33); // 32ビット + 終端文字
        if (!binary_str) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        for (int i = 31; i >= 0; i--) {
            binary_str[31 - i] = (number & (1 << i)) ? '1' : '0';
        }
        binary_str[32] = '\0'; // 終端文字を追加

        //printf("binary_str: %s\n", binary_str);
        return binary_str;
    }
    // //printf("imm: %s\n",imm);
    if (is_float(imm)){
        float imm_value = atof(imm);
        return float_to_binary(imm_value);
    } else {
        int imm_value = atoi(imm);
        return int_to_binary(imm_value);
    }
}


//文字列をスライス
//[a:b]の時はstart = strle(source)-(a+1), length = b-a+1
void get_substring(const char* source, char* destination, int start, int length){
    strncpy(destination, source+start, length);
    destination[length] = '\0';
}

typedef struct {
    char label[256];
    int position;
} LabelEntry;

LabelEntry labels[MAX_LABELS];
int label_count = 0;

LabelEntry found_labels(const char* assembly_code){
    const char* delimiter = "\n";
    char* code_copy = strdup(assembly_code);
    char* token = strtok(code_copy, delimiter);
    int line_number = 0;

    while(token != NULL){
        char *colon = strchr(token, ':');
        if(colon != NULL){
            *colon = '\0';
            strcpy(labels[label_count].label,token);
            labels[label_count].position = line_number+1;
            label_count++;
            // printf("label_name:%s, position:%d\n",token,line_number+1);
        }
        token = strtok(NULL,delimiter);
        line_number++;
    }

    free(code_copy);
}

void trim_whitespace(char* str){
    char* end;
    while(isspace((unsigned char)* str)) str++;

    if(*str == 0){
        *str = '\0';
        return;
    }
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int calculate_offset(const char* assembly_code, const char* label_name, int current_line) {
    //print("current_line:%d\n",current_line);
    const char* line_start = assembly_code; // 現在の行の開始位置(ポインタ)
    int line_number = 0;
   //print("%s\n",label_name);
    size_t label_length = strlen(label_name);
    
    char trimmed_label[256];
    strncpy(trimmed_label, label_name, sizeof(trimmed_label) - 1);
    trimmed_label[sizeof(trimmed_label) - 1] = '\0'; // 終端を保証
    trim_whitespace(trimmed_label);
    label_length = strlen(trimmed_label);

    // 構造体LabelEntryの中にlabelがあったらそこの位置をreturn
    for(int i=0; i<label_count; i++){
        if(strcmp(labels[i].label, trimmed_label) == 0){
            // printf("position:%d\n",labels[i].position - 1);
            return (labels[i].position - current_line - 1) * 4;
        }
    }

    //print("Label: %s, Length: %zu\n", label_name, label_length);
    while (*line_start != '\0') {
        //printf("%s",line_start);
        const char* line_end = strchr(line_start, '\n'); // 次の行の終わりを探す
        if (line_end == NULL) {
            line_end = line_start + strlen(line_start); // 最後の行の場合
        }
        size_t line_length = line_end - line_start;
       //printf("%ld\n",line_length);

        // 行の先頭にラベルがあり、コロンで終わっているかを確認
        if (line_length >= label_length && 
            strncmp(line_start, label_name, label_length) == 0 && 
            (line_start[label_length] == ':')) {
            //print("%c",line_start[label_length-1]);
           //print("hit\n");
        //    printf("line_number:%d\n",line_number);
           //print("offset 204  %d\n",line_number - current_line);
            return (line_number - current_line) * 4; // オフセットを計算
        }
        // 次の行へ進む
        line_start = (*line_end == '\n') ? line_end + 1 : line_end;
        line_number++;
    }
    return 1;
}

    //レジスタセットに対応
    // - `x0`: `zero` - 常に値が0のレジスタ
    // - `x1`: `ra` - リターンアドレス (関数から戻るときのアドレス)
    // - `x2`: `sp` - スタックポインタ (スタック領域を管理する)
    // - `x10 ~ x17`: `a0 ~ a7` - 関数の引数または戻り値
    // - `x5 ~ x7`: `t0 ~ t2` - 一時レジスタ（関数呼び出し間で保存されない）
    // - 'f0 ~ f31' : 'x32 ~ x63'

void change(char *operand, const char *register_name, const char *x_register_name) {
    char *found = strstr(operand, register_name);
    if (found != NULL) {
        size_t reg_len = strlen(register_name);
        size_t x_reg_len = strlen(x_register_name);
        memmove(found + x_reg_len, found + reg_len, strlen(found + reg_len) + 1);
        memcpy(found, x_register_name, x_reg_len);
    }
    for(int i = 0; i <= 32; i++){
        char reg_name[4];
        char x_reg_name[4];
        sprintf(reg_name, "f%d", i);
        sprintf(x_reg_name, "x%d", 32 + i);
        change(operand, reg_name, x_reg_name);
    } 
}
// 使用したregister
int use_register[64] = {0};

void using_register(char* operand){
    if(strncmp(operand,"x",1) ==  0){
        //printf("using %s\n",operand);
        int reg_number = atoi(operand+1); //"x"の後の数字を取得
        if(reg_number >= 0 && reg_number < 64){
            use_register[reg_number]++;
        }
    }
}

void parse_assembly(const char* assembly_code){
    const char* delimiter = "\n";
    char* code_copy = strdup(assembly_code);
    char* token = strtok(code_copy, delimiter);
    BinaryInstruction inst;
    memset(binary_instructions, 0, sizeof(binary_instructions));

    while (token != NULL){
        // printf("token:%s\n",token);
        //printf("instruction_count:%d\n",instruction_count);
        //labelの部分は0000...0で出力するようになっている
        if (strchr(token, ':') != NULL ||
            strstr(token, ".globl") != NULL){
            //token = strtok(NULL, "\n");
            instruction_count++;
            snprintf(inst.binary_code, sizeof(inst.binary_code),"00000000000000000000000000000000");
            binary_instructions[binary_instruction_count++] = inst;
            token = strtok(NULL,delimiter);
            continue;
        }
        //finishは1111...1で出力
        if (strstr(token, "finish") != NULL){
            instruction_count++;
            snprintf(inst.binary_code, sizeof(inst.binary_code),"11111111111111111111111111111111");
            binary_instructions[binary_instruction_count++] = inst;
            token = strtok(NULL,delimiter);
            continue;
        }
        if (strstr(token, "break") != NULL){
            instruction_count++;
            snprintf(inst.binary_code, sizeof(inst.binary_code),"11111111111111111111111111111110");
            binary_instructions[binary_instruction_count++] = inst;
            token = strtok(NULL,delimiter);
            continue;
        }
        char opcode[16],operand1[32],operand2[64],operand3[32];
        memset(opcode, 0, sizeof(opcode));
        memset(operand1, 0, sizeof(operand1));
        memset(operand2, 0, sizeof(operand2));
        memset(operand3, 0, sizeof(operand3));
        // //printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);
        // カンマと空白を区切り文字として扱う
        sscanf(token, "%s %[^,], %[^,], %s", opcode, operand1, operand2, operand3);
        // //printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);

        // 疑似命令に対応(mv, li, ret, j, fsw, flw, bgt, fmv)
        if(strcmp(opcode, "mv") == 0){
            strcpy(opcode, "addi");
            strcpy(operand3, "0");
        }        
        if(strcmp(opcode, "fmv") == 0){
            strcpy(opcode, "addi");
            strcpy(operand3, "0");
        }
        if(strcmp(opcode, "li") == 0){
            strcpy(opcode, "addi");
            strcpy(operand3, operand2);
            strcpy(operand2, "x0");
        }
        if(strcmp(opcode, "ret") == 0){
            strcpy(opcode, "jalr");
            strcpy(operand1, "x0");
            strcpy(operand2, "x1");
            strcpy(operand3, "0");
        }
        if(strcmp(opcode, "j") == 0){
            strcpy(opcode, "jal");
            strcpy(operand2,operand1);
            strcpy(operand1, "x0");
        }
        if(strcmp(opcode, "flw") == 0){
            strcpy(opcode, "lw");
        }
        if(strcmp(opcode, "fsw") == 0){
            strcpy(opcode, "sw");
        }
        if(strcmp(opcode, "bgt") == 0){
            strcpy(opcode, "blt");
            char temp[10];
            strcpy(temp, operand1);
            strcpy(operand1, operand2);
            strcpy(operand2,temp);
        }

        //レジスタセットに対応
        convert_registerset_to_x(operand1);
        convert_registerset_to_x(operand2);
        convert_registerset_to_x(operand3);
        // printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);     
        using_register(operand1);
        using_register(operand2);
        using_register(operand3);

        const char* opcode_bin = get_opcode_binary(opcode);//opcodeを生成 add -> r_type -> 0001
        char* rd_bin = get_register_binary(operand1); 
        int not_rd_free = 0;
        if(!strchr(operand1, 'x')){
            //0ならfree
            not_rd_free = 1;
        }
        char* r1_bin;
        int need_free_reg_1 = 0;
        int need_free_imm_1 = 0;
        if(operand2[0] == 'x'){ //operand2がレジスタ
            //printf("need_free_reg_1");
            r1_bin = get_register_binary(operand2);
            need_free_reg_1 = 1;
        }else if(operand2[0] != '\0'){
            //printf("operand2:%s\n",operand2);
            if(strcmp(opcode, "sw") == 0 || strcmp(opcode, "lw") == 0){
            } else {
                // //printf("need_free_imm_1\n");
                r1_bin = get_immediate_binary(operand2);
                need_free_imm_1 = 1;
            }
        }
        char* r2_bin;
        int need_free_reg_2 = 0;
        int need_free_imm_2 = 0;
        if(operand3[0] == 'x'){
            r2_bin = get_register_binary(operand3);
            need_free_reg_2 = 1;
        }else if(operand3[0] != '\0'){
            if(strcmp(opcode, "sw") == 0 || strcmp(opcode, "lw") == 0){
            } else {
                //printf("need_free_imm_2\n");
                r2_bin = get_immediate_binary(operand3);
                need_free_imm_2 = 1;
            }
        }
        ////printf("free_reg:%d %d %d %d\n",need_free_imm_1,need_free_imm_2,need_free_reg_1,need_free_reg_2);
        //printf("op_bin:%s, rd_bin:%s, r1_bin:%s, r2_bin:%s\n",opcode_bin,rd_bin,r1_bin,r2_bin);
        //printf("%d,%d,%d,%d\n",need_free_imm_1,need_free_imm_2,need_free_reg_1,need_free_reg_2);

        if(strcmp(opcode, "la") == 0 || strcmp(opcode, "la_1") == 0){
            //printf("opcode:%s\n",opcode);
            // la rd, symbolを変換すると下2行に対応
            // auipc rd, symbol(31:12)
            // uaddi rd, rd, symbol(11:0)
            int current_line = instruction_count;
            int operand2_num = atoi(operand2); // Assuming operand2 is a string that represents a number
            //printf("operand2:%d\n", operand2_num);
            const char* label_name = operand2;
            //printf("label_name:%s\n", label_name);
            int offset = 0;
            if (strcmp(opcode, "la") == 0) {
                if(strstr(label_name,"fast") != NULL){
                    printf("label_name:%s\n",label_name);
                    printf("offset:%d\n",offset+4);
                }
                offset = calculate_offset(assembly_code, label_name, current_line);
            }
            if (strcmp(opcode, "la_1") == 0) {
                offset = operand2_num + 12;
            }
            // offset = offset + current_line * 4 + 4; //laが3命令に分割する
            // auipcを使用 -> current_line足さない
            offset = offset + 4;
            // offset = offset + 8; //laが3命令に分割する
            // オフセットを32ビット符号付き整数として扱う
            int32_t signed_offset = offset;
            // 上位20ビットを取得 (符号付きで右シフト)
            int32_t bit31_12 = signed_offset >> 12; 
            // 下位12ビットを取得
            uint32_t bit11_0 = signed_offset & 0xFFF;
            // 下位12ビットの符号拡張を確認
            if (bit11_0 & 0x800) {
                bit11_0 |= 0xFFFFF000; // 32ビットの符号拡張
            }    
            char bit31_12_bin[21] = {0};
            char bit11_0_bin[13] = {0};

            for (int i = 0; i < 20; i++) {
                bit31_12_bin[19 - i] = (bit31_12 & (1 << i)) ? '1' : '0';
            }

            for (int i = 0; i < 12; i++) {
                bit11_0_bin[11 - i] = (bit11_0 & (1 << i)) ? '1' : '0';
            }

            //printf("bit31_12 (2進数): %s\n", bit31_12_bin);
            //printf("bit11_0 (2進数): %s\n", bit11_0_bin);
            // //printf("%s00%s0110\n", bit31_12, rd_bin);
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s0110", bit31_12_bin, rd_bin);
            //lui version(多分違う)
            // snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s0101 ", bit31_12_bin, rd_bin);

            // nop(addi x0,x0,0)命令を追加
            instruction_count++;
            binary_instructions[binary_instruction_count++] = inst;
            snprintf(inst.binary_code, sizeof(inst.binary_code),"00000000000000000000000000000010");

            instruction_count++;
            binary_instructions[binary_instruction_count++] = inst;
            // char bit11_0[13];
            // get_substring(r1_bin, bit11_0, strlen(r1_bin)-12, 12);
            // //printf("bit11_0:%s\n",bit11_0);
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s110%s0010", bit11_0_bin, rd_bin, rd_bin);
            }

        if(strcmp(opcode, "li_1") == 0){
            // liの即値が32bit
            // printf("r1_bin:%s\n",r1_bin);
            // lui rd imm(31:12)
            // uaddi rd, rd, imm(11:0)

            char bit31_12[21];
            char bit11_0[13];
            get_substring(r1_bin,bit31_12,strlen(r1_bin)-32,20);
            get_substring(r1_bin,bit11_0,strlen(r1_bin)-12,12);
            // printf("bit31_12:%s\n",bit31_12);
            // printf("bit11_0:%s\n",bit11_0);

            //lui rd imm(31:12)
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s0101", bit31_12, rd_bin);

            instruction_count++;
            binary_instructions[binary_instruction_count++] = inst;
            // uaddi rd, rd, imm(11:0)
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s110%s0010", bit11_0, rd_bin, rd_bin);
        }

        if(is_r_type(opcode)){
            // printf("r_type\n");
            if(strcmp(opcode, "add") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "sub") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0100000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "and") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s111%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            // if(strcmp(opcode, "or") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s110%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "xor") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s100%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "div10") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000000000%s011%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "rem") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s110%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "sll") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s001%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "sra") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0100000%s%s101%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "srl") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s101%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
        }
        if(is_i_type(opcode)){
            //printf("i_type\n");
            //[11:0]
            char r2_bin_sub[13];//12bit + 終端文字
            get_substring(r2_bin,r2_bin_sub,strlen(r2_bin)-12,12);
            if(strcmp(opcode, "addi") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s000%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
            if(strcmp(opcode, "uaddi") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s110%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
            if(strcmp(opcode, "andi") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s111%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
            // if(strcmp(opcode, "ori") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s110%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
            if(strcmp(opcode, "xori") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s100%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
            if(strcmp(opcode, "slli") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code), "%s0%s001%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
        }
        if (is_s_type(opcode)) {
            const char *op_start = operand2; //x0(0)
            //swの処理
            //sw x8 x0(0)
            convert_registerset_to_x(operand2);
            char *x = strchr(operand2, 'x');
            char *start = strchr(operand2, '(');
            char *end = strchr(operand2, ')');
            char offset[10];
            char *offset_ptr = offset;
            while (op_start < start) {
                *offset_ptr++ = *op_start++;
            }
            *offset_ptr = '\0';
            char reg_num[5];
            char *reg_ptr = reg_num;
            while (x < end) {
                *reg_ptr++ = *x++;
            }
            *reg_ptr = '\0';

            char *offset_bin = get_immediate_binary(offset);
            char *r1_bin = get_register_binary(reg_num);

            char bit11_5[8], bit4_0[6];
            get_substring(offset_bin, bit11_5, strlen(offset_bin) - 12, 7);
            get_substring(offset_bin, bit4_0, strlen(offset_bin) - 5, 5);

            snprintf(inst.binary_code, sizeof(inst.binary_code), "%s%s%s0100%s%s", bit11_5, rd_bin, r1_bin, bit4_0, opcode_bin);
        }
        if(is_b_type(opcode)){
            //printf("b_type\n");
            //offsetを求める
            int current_line = instruction_count;
            const char* label_name = operand3;
            int offset = calculate_offset(assembly_code,label_name,current_line);
            char offset_str[20];
            //printf("offset:%d\n",offset);
            snprintf(offset_str,sizeof(offset_str),"%d",offset);
            r2_bin = get_immediate_binary(offset_str);
            //printf("%s\n",r2_bin);

            //opcode -> funct3, rd -> r1, r1 -> r2, r2 -> offsetに対応
            char imm_1[8],imm_2[6];
            char bit12[2],bit10_5[7],bit4_1[5],bit11[2];

            get_substring(r2_bin,bit12,strlen(r2_bin)-13,1);
            get_substring(r2_bin,bit10_5,strlen(r2_bin)-11,6);
            snprintf(imm_1, 8, "%s%s", bit12, bit10_5); // 12ビットと10:5ビットを結合
            get_substring(r2_bin,bit4_1,strlen(r2_bin)-5,4);
            get_substring(r2_bin,bit11,strlen(r2_bin)-12,1);
            snprintf(imm_2, 6, "%s%s", bit4_1, bit11); // 4:

            if(strcmp(opcode, "beq") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s0000%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "bne") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s0010%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "blt") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s1000%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "bge") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s1010%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "bgt") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s1100%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "bltu") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s1100%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            if(strcmp(opcode, "bgeu") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s1110%s%s", imm_1, r1_bin, rd_bin, imm_2, opcode_bin);
            //printf("end");
        }
        if(is_u_type(opcode)){
            //printf("u_type\n");
            //lui rd upimm
            //printf("rd_bin:%s\n",rd_bin);
            //printf("r1_bin:%s\n",r1_bin);
            char bit31_12[21];
            get_substring(r1_bin, bit31_12, strlen(r1_bin)-20, 20);
            //printf("bit31_12:%s\n",bit31_12);
            if(strcmp(opcode, "lui") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s%s", bit31_12, rd_bin, opcode_bin);
            //printf("end");
        }
        if(is_aui_type(opcode)){
            //auipc
            char bit31_12[21];
            get_substring(r1_bin, bit31_12, strlen(r1_bin)-32, 20);
            if(strcmp(opcode, "auipc") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s%s", bit31_12, rd_bin, opcode_bin);
        }
        if(is_j_type(opcode)){
           //printf("j_type\n");

           // j labelとなっているときは
           //label_name = operand1, rd_bin = get_register_binary("x1")
           // j register labelとなっている時は
           // lable_name = operand2, rd_bin = get_register_binary(opernad1)に変更
            //offsetを求める
            int current_line = instruction_count;
            const char* label_name = operand2;
            // //printf("label_name 528 : %s\n",label_name);
           //printf("label: %s\n",label_name);
            int offset = calculate_offset(assembly_code,label_name,current_line);
        //    //printf("offset 325  %d\n",offset);
            char offset_str[20];
            snprintf(offset_str,sizeof(offset_str),"%d",offset);
            r1_bin = get_immediate_binary(offset_str);
            need_free_imm_1 = 1;
            //printf("offset:%d\n",offset);
            rd_bin = get_register_binary(operand1);
            //printf("rd_bin:%s\n",rd_bin);

            //r1 -> offsetに対応
            char bit20[2],bit10_1[11],bit11[2],bit19_12[9];
            char imm[21];
            
            get_substring(r1_bin,bit20,strlen(r1_bin)-21,1);
            get_substring(r1_bin,bit10_1,strlen(r1_bin)-11,10);
            get_substring(r1_bin,bit11,strlen(r1_bin)-12,1);
            get_substring(r1_bin,bit19_12,strlen(r1_bin)-20,8);
            snprintf(imm, 21,"%s%s%s%s", bit20, bit10_1, bit11, bit19_12);
            
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s00%s%s",imm, rd_bin, opcode_bin);
        }
        if(is_jalr_type(opcode)){
            char r2_bin_sub[13];//12bit + 終端文字
            get_substring(r2_bin,r2_bin_sub,strlen(r2_bin)-12,12);
            if(strcmp(opcode, "jalr") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"%s0%s000%s%s", r2_bin_sub, r1_bin, rd_bin, opcode_bin);
        }
        if (is_lw_type(opcode)) {
            const char *op_start = operand2; //x0(0)
            //lwの処理
            //lw x8 x0(0)
            convert_registerset_to_x(operand2);
            char *x = strchr(operand2, 'x');
            char *start = strchr(operand2, '(');
            char *end = strchr(operand2, ')');
            char offset[10];
            char *offset_ptr = offset;
            while (op_start < start) {
                *offset_ptr++ = *op_start++;
            }
            *offset_ptr = '\0';
            char reg_num[5];
            char *reg_ptr = reg_num;
            while (x < end) {
                *reg_ptr++ = *x++;
            }
            *reg_ptr = '\0';

            char *offset_bin = get_immediate_binary(offset);
            char *r1_bin = get_register_binary(reg_num);

            char bit11_0[13];
            get_substring(offset_bin, bit11_0, strlen(offset_bin) - 12, 12);

            snprintf(inst.binary_code, sizeof(inst.binary_code), "%s0%s010%s%s", bit11_0, r1_bin, rd_bin, opcode_bin);
        }
        if(is_f_type(opcode)){
            //printf("f_type\n");
            //丸めモード: 最近傍
            // funct7[31:25] | rs2[24:19] | rs1[18:13] | funct3[12:10] | rd[9:4] | op[3:0]
            // funct7: { [fadd, 00000 00], [fsub, 00001 00], [fmul, 00010 00], [fdiv, 00011 00], [fsqrt, 01011 00], [fsgnjn, 00100 00], [fsgnjx, 00100 00],
            // [feq, 10100 00], [flt, 10100 00], [fcvtws, 11000 00], [fcvtsw, 11010 00], [floor, 11100 00]}
            // funct3: { [fadd, 000], [fsub, 000], [fmul, 000], [fdiv, 000], [fsqrt, 000], [fsgnjn, 001], [fsgnjx, 010],
            // [feq, 010], [flt, 001], [fcvtws, 000], [fcvtsw, 000], [floor, 000]}
            snprintf(inst.binary_code,sizeof(inst.binary_code),"%s%s%s000%s1010011",opcode_bin,r2_bin,r1_bin,rd_bin);
            if(strcmp(opcode, "fadd") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fsub") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000100%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fmul") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0001000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fdiv") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0001100%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fabs") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0110000000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fneg") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0110100000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "finv") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0101000000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fsqrt") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0101100000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fsgnjn") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0010000%s%s001%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fsgnjx") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0010000%s%s010%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "feq") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"1010000%s%s010%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "flt") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"1010000%s%s001%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fcvtws") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"1100000000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "fcvtsw") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"1101000000000%s000%s%s",r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "floor") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"1110000000000%s000%s%s",r1_bin,rd_bin, opcode_bin);

        }
        if(is_c_type(opcode)){
            //x10の値をファイルに書き込む
            if(strcmp(opcode, "csrw_int") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s0000001000000001011",rd_bin);
            //printf("operand1;%s\n",rd_bin);
            if(strcmp(opcode, "csrw") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s0000000010000001011",rd_bin);
            //sldファイルの値をx10に書き込む
            if(strcmp(opcode, "csrr") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000000000000000010%s1011",rd_bin);

        }
        // printf("Binary Code: %s\n",inst.binary_code);
        // //printf("before");
        instruction_count++;
        ////printf("after_instruction_count");
        binary_instructions[binary_instruction_count++] = inst;
        ////printf("hello");
        if(not_rd_free == 0){
            free((void*)rd_bin);
        }
        if(need_free_reg_1 == 1 || need_free_imm_1 == 1){
            //free(r1_bin);
        }        
        if(need_free_reg_2 == 1 || need_free_imm_2 == 1){
            //free(r2_bin);
        }
        //printf("before_token:%s\n",token);
        char* before_token = token;
        token = strtok(NULL,delimiter);
        // printf("after_token:%s\n",token);
        //　li_1は2命令に分かれるから調節
        if(strstr(before_token, "li_1") != NULL){
            token = strtok(NULL,delimiter);
            //printf("after_token:%s\n",token);
        }
        // laとla_1はauipcとaddiの3命令に分かれるから調節
        if(strstr(before_token, "la") != NULL && strstr(token, "la") != NULL){
            token = strtok(NULL,delimiter);
            //printf("after_token:%s\n",token);
            token = strtok(NULL,delimiter);
            //printf("after_token:%s\n",token);
        }
        if(strstr(before_token, "la_1") != NULL && strstr(token, "la_1") != NULL){
            token = strtok(NULL,delimiter);
            //printf("after_token:%s\n",token);
            token = strtok(NULL,delimiter);
            //printf("after_token:%s\n",token);
        }
    }
    free(code_copy);
}

void remove_comments(char *code) {
    char *src = code;
    char *dst = code;
    int in_comment = 0;

    while (*src) {
        if (*src == ';' || *src == '#') {
            in_comment = 1;
        }
        if (*src == '\n') {
            in_comment = 0;
        }
        if (!in_comment) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

// すべてのバイナリ命令を出力
int print_binary_instructions(FILE* output_file) {
    int instruction_count = binary_instruction_count;
    char instruction_count_binary_code[32];
    for (int i = 30; i >= 0; --i) {
        instruction_count_binary_code[i] = (binary_instruction_count % 2) + '0';
        binary_instruction_count /= 2;
    }
    instruction_count_binary_code[31] = '\0'; // 終端文字を追加
    // 行数表示
    //fprintf(output_file, "%s\n", instruction_count_binary_code);
    for (int i = 0; i < instruction_count; i++) {
        fprintf(output_file, "%s\n", binary_instructions[i].binary_code);
    }
    return instruction_count;
}

int parse_main(){
    FILE *file;
    char assembly_code[MAX_ASSEMBLY_SIZE];

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

    remove_comments(assembly_code);

    parse_assembly(assembly_code);

    FILE *output_file = fopen("binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int count = print_binary_instructions(output_file);
    fclose(output_file);

    return 0;
}