#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//アセンブリコードをバイナリコードにparse

#define MAX_LENGTH 100
#define MAX_ASSEMBLY_SIZE 1024 //アセンブリコードの最大サイズ

int instruction_count = 0;

typedef struct{
    char binary_code[33];
} BinaryInstruction;

BinaryInstruction binary_instructions[MAX_LENGTH];
int binary_instruction_count = 0;

// mv ret li
// mv a b = add a, x0, b(a = b)
// li a imm = addi a, x0, imm(a = imm)
// ret = jalr x0, x1, 0

const char* r_type_opcodes[] = {"add", "sub", "and", "or", "xor", NULL};
const char* i_type_opcodes[] = {"addi", "andi", "ori", "xori","jalr", NULL};
const char* b_type_opcodes[] = {"beq", "bne", "blt", "bge", "bltu", "bgeu", NULL};
const char* j_type_opcodes[] = {"jal", NULL};
const char* s_type_opcodes[] = {"sw","lw"};
const char* f_type_opcodes[] = {"fadd", "fsub", "fmul", "fdiv", NULL};

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

int is_s_type(const char* opcode){
    return is_opcode_type(opcode,s_type_opcodes);
}

int is_f_type(const char* opcode){
    return is_opcode_type(opcode,f_type_opcodes);
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
    if(strcmp(opcode,"jalr") == 0) return "000";

    //S
    //imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode
    if(strcmp(opcode,"sw") == 0) return "0100011";
    if(strcmp(opcode,"lw") == 0) return "0000011";

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

    //F
    // opcode | rs2 | rs1 | rm | rd | 1010011
    if(strcmp(opcode,"fadd") == 0) return "0000000";
    if(strcmp(opcode,"fsub") == 0) return "0000100";
    if(strcmp(opcode,"fmul") == 0) return "0001000";
    if(strcmp(opcode,"fdiv") == 0) return "0001100"; 
}

char* get_register_binary(const char* reg) {
    char* binary = (char*)malloc(6*sizeof(char));
    if(binary == NULL){
        return NULL;
    } 
    int reg_num;
    if (sscanf(reg, "x%d", &reg_num) != 1 || reg_num < 0 || reg_num > 31) {
        free(binary);
        return "00000"; 
    }
    // 2進数に変換する
    for (int i = 4; i >= 0; --i) {
        binary[i] = (reg_num % 2) + '0';
        reg_num /= 2;
    }
    binary[5] = '\0'; // 終端文字を追加
    return binary;
}

// intをbinaryに
char* int_to_binary(int value){
    printf("value: %d\n",value);
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
    printf("imm: %s\n",imm);
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

int calculate_offset(const char* assembly_code, const char* label_name, int current_line) {
    const char* line_start = assembly_code; // 現在の行の開始位置(ポインタ)
    int line_number = 0;
    size_t label_length = strlen(label_name);
    while (*line_start != '\0') {
        const char* line_end = strchr(line_start, '\n'); // 次の行の終わりを探す
        if (line_end == NULL) {
            line_end = line_start + strlen(line_start); // 最後の行の場合
        }
        size_t line_length = line_end - line_start;

        // 行の先頭にラベルがあり、コロンで終わっているかを確認
        if (line_length > label_length && 
            strncmp(line_start, label_name, label_length) == 0 && 
            line_start[label_length] == ':') {
            return (line_number - current_line)*4; // オフセットを計算
        }
        // 次の行へ進む
        line_start = (*line_end == '\n') ? line_end + 1 : line_end; // 改行がある場合は次の行に移動
        line_number++;
    }

    return 0; // ラベルが見つからない場合
}

    //レジスタセットに対応
    // - `x0`: `zero` - 常に値が0のレジスタ
    // - `x1`: `ra` - リターンアドレス (関数から戻るときのアドレス)
    // - `x2`: `sp` - スタックポインタ (スタック領域を管理する)
    // - `x10 ~ x17`: `a0 ~ a7` - 関数の引数または戻り値
    // - `x5 ~ x7`: `t0 ~ t2` - 一時レジスタ（関数呼び出し間で保存されない）

char change(char *operand, char *register_name, char *x_register_name){
    if(strcmp(operand, register_name) == 0){
        strcpy(operand, x_register_name);
    }
}
void convert_registerset_to_x(char *operand){
    change(operand, "zero", "x0");
    change(operand, "ra", "x1");
    change(operand, "sp", "x2");
    for(int i = 0; i <= 7; i++){
        char reg_name[4];
        char x_reg_name[4];
        sprintf(reg_name, "a%d", i);
        sprintf(x_reg_name, "x%d", 10 + i);
        change(operand, reg_name, x_reg_name);
    }
    for(int i = 0; i <= 2; i++){
        char reg_name[4];
        char x_reg_name[4];
        sprintf(reg_name, "t%d", i);
        sprintf(x_reg_name, "x%d", 5 + i);
        change(operand, reg_name, x_reg_name);
    } 
}


void parse_assembly(const char* assembly_code){
    const char* delimiter = "\n";
    char* code_copy = strdup(assembly_code);
    char* token = strtok(code_copy, delimiter);

    while (token != NULL){
        //labelの部分は0000...0で出力するようになっている
        if(strchr(token, ':') != NULL){
            BinaryInstruction inst;
            //token = strtok(NULL, "\n");
            instruction_count++;
            snprintf(inst.binary_code, sizeof(inst.binary_code),"00000000000000000000000000000000");
            binary_instructions[binary_instruction_count++] = inst;
            token = strtok(NULL,delimiter);
            continue;
        }
        char opcode[16],operand1[32],operand2[32],operand3[32];
        memset(opcode, 0, sizeof(opcode));
        memset(operand1, 0, sizeof(operand1));
        memset(operand2, 0, sizeof(operand2));
        memset(operand3, 0, sizeof(operand3));
        printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);
        // カンマと空白を区切り文字として扱う
        sscanf(token, "%s %[^,], %[^,], %s", opcode, operand1, operand2, operand3);
        printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);

        // 疑似命令に対応(mv, li, ret)
        if(strcmp(opcode, "mv") == 0){
            strcpy(opcode, "add");
            strcpy(operand3, operand2);
            strcpy(operand2, "x0");
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

        //レジスタセットに対応
        convert_registerset_to_x(operand1);
        convert_registerset_to_x(operand2);
        convert_registerset_to_x(operand3); 
        printf("opcode:%s, operand1:%s, operand2:%s, operand3:%s\n",opcode,operand1,operand2,operand3);     

        const char* opcode_bin = get_opcode_binary(opcode);
        char* rd_bin = get_register_binary(operand1);
        char* r1_bin;
        int need_free_reg_1 = 0;
        int need_free_imm_1 = 0;
        if(operand2[0] == 'x'){
            printf("need_free_reg_1");
            r1_bin = get_register_binary(operand2);
            need_free_reg_1 = 1;
        }else if(operand2[0] != '\0'){
            printf("need_free_imm_1");
            r1_bin = get_immediate_binary(operand2);
            need_free_imm_1 = 1;
        }
        char* r2_bin;
        int need_free_reg_2 = 0;
        int need_free_imm_2 = 0;
        if(operand3[0] == 'x'){
            r2_bin = get_register_binary(operand3);
            need_free_reg_2 = 1;
        }else if(operand2[0] != '\0'){
            r2_bin = get_immediate_binary(operand3);
            need_free_imm_2 = 1;
        }
        //printf("free_reg:%d %d %d %d\n",need_free_imm_1,need_free_imm_2,need_free_reg_1,need_free_reg_2);
        printf("op_bin:%s, rd_bin:%s, r1_bin:%s, r2_bin:%s\n",opcode_bin,rd_bin,r1_bin,r2_bin);
        //printf("%d,%d,%d,%d\n",need_free_imm_1,need_free_imm_2,need_free_reg_1,need_free_reg_2);

        BinaryInstruction inst;

        if(is_r_type(opcode)){
            printf("r_type\n");
            if(strcmp(opcode, "add") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "sub") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0100000%s%s000%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "and") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s111%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "or") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s110%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
            if(strcmp(opcode, "xor") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code),"0000000%s%s100%s%s",r2_bin,r1_bin,rd_bin, opcode_bin);
        }

        if(is_i_type(opcode)){
            printf("i_type\n");
            //[11:0]
            char r2_bin_sub[13];//12bit + 終端文字
            get_substring(r2_bin,r2_bin_sub,strlen(r2_bin)-12,12);
            char opcode[8];
            if (strcmp(opcode, "jalr") == 0) {
                opcode[0] = '1';
                opcode[1] = '1';
                opcode[2] = '0';
                opcode[3] = '0';
                opcode[4] = '1';
                opcode[5] = '1';
                opcode[6] = '1';
                opcode[7] = '\0';
            } else {
                opcode[0] = '0';
                opcode[1] = '0';
                opcode[2] = '1';
                opcode[3] = '0';
                opcode[4] = '0';
                opcode[5] = '1';
                opcode[6] = '1';
                opcode[7] = '\0';
            }
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s%s%s",r2_bin_sub,r1_bin,opcode_bin,rd_bin,opcode);
        }
        //printf("binary: %s\n",inst.binary_code);

        if(is_s_type(opcode)){
            printf("s_type\n");
            // lw x8, 4(x6)
            // rd->r2に対応
            const char *op_start = operand2;
            char *x;
            x = strchr(operand2,'x');
            char *start;
            start = strchr(operand2,'(');
            char *end;
            end = strchr(operand2,')');
            char offset[10];
            char *offset_ptr = offset;
            while(op_start < start){
                *offset_ptr++ = *op_start++;
            }
            *offset_ptr = '\0';
            char reg_num[4];
            char *reg_ptr = reg_num;
            while(x<end){
                *reg_ptr++ = *x++;
            }
            *reg_ptr = '\0';

            char *offset_bin = get_immediate_binary(offset);
            char *r1_bin = get_register_binary(reg_num);

            char bit11_5[8],bit4_0[6];
            char bit11_0[13];
            get_substring(offset_bin,bit11_5,strlen(offset_bin)-12,7);
            get_substring(offset_bin,bit4_0,strlen(offset_bin)-5,5);
            get_substring(offset_bin,bit11_0,strlen(offset_bin)-12,12);
            //printf("offset%s,bit11_5:%s,bit4_0:%s,rd:%s\n",bit11_5,bit4_0,r1_bin,rd_bin);

            if(strcmp(opcode, "sw") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code), "%s%s%s010%s%s",bit11_5,rd_bin,r1_bin,bit4_0,opcode_bin);
            if(strcmp(opcode, "lw") == 0) snprintf(inst.binary_code, sizeof(inst.binary_code), "%s%s010%s%s", bit11_0,r1_bin,rd_bin,opcode_bin);
        }

        if(is_b_type(opcode)){
            printf("b_type\n");
            //offsetを求める
            int current_line = instruction_count;
            const char* label_name = operand3;
            int offset = calculate_offset(assembly_code,label_name,current_line);
            char offset_str[20];
            snprintf(offset_str,sizeof(offset_str),"%d",offset);
            r2_bin = get_immediate_binary(offset_str);

            //opcode -> funct3, rd -> r1, r1 -> r2, r2 -> offsetに対応
            char imm_1[8],imm_2[6];
            char bit12[2],bit10_5[7],bit4_1[5],bit11[2];

            get_substring(r2_bin,bit12,strlen(r2_bin)-13,1);
            get_substring(r2_bin,bit10_5,strlen(r2_bin)-11,6);
            snprintf(imm_1, 8, "%s%s", bit12, bit10_5); // 12ビットと10:5ビットを結合
            get_substring(r2_bin,bit4_1,strlen(r2_bin)-5,4);
            get_substring(r2_bin,bit11,strlen(r2_bin)-12,1);
            snprintf(imm_2, 6, "%s%s", bit4_1, bit11); // 4:

            size_t buffer_size = sizeof(inst.binary_code); // バッファのサイズを取得
            if (buffer_size > 0) {
                snprintf(inst.binary_code, buffer_size, "%s%s%s%s%s1100011", imm_1, r1_bin, rd_bin, opcode_bin, imm_2);
            } else {
                fprintf(stderr, "Error: Binary code buffer size is zero or negative.\n");
            }

        }

        if(is_j_type(opcode)){
            printf("j_type\n");
            //offsetを求める
            int current_line = instruction_count;
            const char* label_name = operand1;
            int offset = calculate_offset(assembly_code,label_name,current_line);
            char offset_str[20];
            snprintf(offset_str,sizeof(offset_str),"%d",offset);
            r1_bin = get_immediate_binary(offset_str);
            need_free_imm_1 = 1;
            printf("offset:%d\n",offset);
            rd_bin = get_register_binary("x1");
            printf("rd_bin:%s\n",rd_bin);

            //r1 -> offsetに対応
            char bit20[2],bit10_1[11],bit11[2],bit19_12[9];
            char imm[21];
            
            get_substring(r1_bin,bit20,strlen(r1_bin)-21,1);
            get_substring(r1_bin,bit10_1,strlen(r1_bin)-11,10);
            get_substring(r1_bin,bit11,strlen(r1_bin)-12,1);
            get_substring(r1_bin,bit19_12,strlen(r1_bin)-20,8);
            snprintf(imm, 21,"%s%s%s%s", bit20, bit10_1, bit11, bit19_12);
            
            snprintf(inst.binary_code, sizeof(inst.binary_code),"%s%s%s",imm, rd_bin, opcode_bin);
        }

        if(is_f_type(opcode)){
            printf("f_type\n");
            //丸めモード: 最近傍
            snprintf(inst.binary_code,sizeof(inst.binary_code),"%s%s%s000%s1010011",opcode_bin,r2_bin,r1_bin,rd_bin);

        }
        
        printf("Binary Code: %s\n",inst.binary_code);
        //printf("before");
        instruction_count++;
        //printf("after_instruction_count");
        binary_instructions[binary_instruction_count++] = inst;
        //printf("hello");
        free((void*)rd_bin);
        if(need_free_reg_1 == 1 || need_free_imm_1 == 1){
            free(r1_bin);
        }        
        if(need_free_reg_2 == 1 || need_free_imm_2 == 1){
            free(r2_bin);
        }
        //printf("before_token:%s\n",token);
        token = strtok(NULL,delimiter);
        //printf("after_token:%s\n",token);
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
void print_binary_instructions(FILE* output_file) {
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
}

int main(){
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

    FILE *output_file = fopen("output.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    print_binary_instructions(output_file);
    fclose(output_file);

    return 0;
}
