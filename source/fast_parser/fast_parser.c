// binary から 実行結果を出力
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include "../float/math/math_functions.h"
#include "../asm_to_binary/asm_to_binary.h"

#define NUM_REGISTERS 64
#define INSTRUCTION_LENGTH 33 //32bit + 終端文字
#define MEMORY_SIZE 8388608
#define STACK_SIZE 4096
#define MAX_ASSEMBLY_SIZE 1048448  // アセンブリコードの最大サイズ
#define MAX_INSTRUCTION_LENGTH 50 // 1行の長さ
#define BUFFER_SIZE 32768

typedef struct {
    char data[BUFFER_SIZE];
    size_t used;
} Buffer;

Buffer memory_buffer = {0};

void flush_buffer(FILE *file, Buffer *buffer) {
    if (buffer->used > 0) {
        fwrite(buffer->data, 1, buffer->used, file);
        buffer->used = 0;
    }
}

void write_to_buffer(FILE *file, Buffer *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int written = vsnprintf(buffer->data + buffer->used, 
                            BUFFER_SIZE - buffer->used, 
                            format, args);
    
    va_end(args);

    if (written < 0) {
        // エラー処理
        return;
    }

    if (buffer->used + written >= BUFFER_SIZE) {
        // バッファがいっぱいになったらフラッシュ
        flush_buffer(file, buffer);
        
        // 書き込みきれなかった分を再度書き込み
        va_start(args, format);
        vsnprintf(buffer->data, BUFFER_SIZE, format, args);
        va_end(args);
        
        buffer->used = strlen(buffer->data);
    } else {
        buffer->used += written;
    }
}

float memory[MEMORY_SIZE];
int sp = MEMORY_SIZE - 1; //スタックポインタ

// registerのsimulation
double registers[NUM_REGISTERS] = {0};  // 64個のレジスタを初期化 0-31は整数　32-63はfloat

// extern InstructionCounter counter;

// レジスタの値を設定するreg_numは0 ~ 63
void set_register(int reg_num, double value) {
    // //printf("reg_num:%d\n",reg_num);
    // printf("value:%lf\n",value);
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < NUM_REGISTERS) {
        registers[reg_num] = value;
    }
}

// レジスタの値を取得する reg_num = 0~31(int version)
static inline int get_register(int reg_num) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < 32) {
        return registers[reg_num];
    }
    //printf("reg[%d] = %d\n",reg_num,registers[reg_num]);
    return 0;
}
//reg_num 32 ~ 63
static inline double get_float_register(int reg_num) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 32 && reg_num < NUM_REGISTERS) {
        return registers[reg_num];
    }
    //printf("reg[%d] = %f\n",reg_num,registers[reg_num]);
    return 0;
}

int32_t read_next_value_from_file(FILE *file, int *is_float) {
    char line[50];
    if (fgets(line, sizeof(line), file)) {
        // 改行文字を削除
        line[strcspn(line, "\n")] = 0;

        // 小数点が含まれているかチェック
        if (strchr(line, '.') != NULL) {
            // 浮動小数点数として解釈
            float f_value = strtof(line, NULL);
            uint32_t i_value;
            memcpy(&i_value, &f_value, sizeof(float));
            *is_float = 1;
            return i_value;
        } else {
            // 整数として解釈
            int32_t i_value = atoi(line);
            *is_float = 0;
            return (uint32_t)i_value;
        }
    }
    *is_float = 0;
    return 0; // ファイルの終わりやエラーの場合
}
  
void handle_float_register(uint32_t rd, uint32_t rs1, int32_t imm) {
    if (0 <= rs1 && rs1 < 32) {
        // rs1が整数レジスタの場合、整数を浮動小数点数に変換
        int32_t int_value = get_register(rs1) + imm;
        float float_value;
        memcpy(&float_value, &int_value, sizeof(float_value));
        set_register(rd, float_value);
    } else {
        // rs1が浮動小数点レジスタの場合、そのままコピー
        float float_value = get_float_register(rs1);
        set_register(rd, float_value);
    }
}

void handle_addi_case(uint32_t rd, uint32_t rs1, int32_t imm) {
    if (0 <= rd && rd < 32) {
        set_register(rd, get_register(rs1) + imm);
    } else {
        handle_float_register(rd, rs1, imm);
    }
}

void print_use_register_transition(FILE *transition_file, int pc, int use_register[64]){
    char buffer[1024];  // バッファを用意
    int offset = 0;
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "| %2d行|", pc);
    
    for(int i = 0; i < 32; i++) {
        if(use_register[i] > 0){
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%3d | ", get_register(i));
        }
    }
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "\n");
    
    // 一括書き出し
    fwrite(buffer, 1, offset, transition_file);
}

void print_use_float_register_transition(FILE *float_transition_file, int pc, int use_register[64]){
    char buffer[1024];
    int offset = 0;

    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "| %2d行|", pc);
    
    for(int i = 32; i < NUM_REGISTERS; i++) {
        if(use_register[i] > 0){
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%3f | ", get_float_register(i));
        }
    }
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "\n");

    // 一括書き出し
    fwrite(buffer, 1, offset, float_transition_file);
}

int handle_r(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t func3){
    // printf("r_type\n");
    uint32_t func7 = (instruction >> 25) & 0x7F;
    double rs1_value = (rs1 < 32) ? get_register(rs1) : get_float_register(rs1);
    double rs2_value = (rs2 < 32) ? get_register(rs2) : get_float_register(rs2);

    if (func3 == 0){
        if (func7 == 0){  // add命令
            set_register(rd, rs1_value + rs2_value);
        } else if (func7 == 0x20) {  // sub命令
            set_register(rd, rs1_value - rs2_value);
        }
    } else if (func3 == 0x7){  // and
        set_register(rd, get_register(rs1) & get_register(rs2));
    } else if (func3 == 0x4){  // xor
        set_register(rd, get_register(rs1) ^ get_register(rs2));
    }  else if (func3 == 0x3){ //div10(商)
        uint32_t digits[4] = {0};
        int num_digits = 0;
        // 10で割って各桁を取得
        while((int)rs1_value > 0 && num_digits < 4){
            digits[num_digits++] = (uint32_t)rs1_value % 10;
            rs1_value /= 10;
        }
        uint32_t result = 0;
        for(int i=0; i<num_digits; i++){
            result |= ((digits[i] + '0') << ((num_digits-i-1)*8 + 2));
        }
        result |= ((num_digits - 1) & 0x3);
        // result <<= (4 - num_digits) * 8;
        set_register(rd, result);
    } else if (func3 == 0x6){ //rem(余り)
        set_register(rd, get_register(rs1) % get_register(rs2));
    } else if (func3 == 0x1){ //shift(sll)
        set_register(rd, get_register(rs1) << (get_register(rs2) & 0x1F));
    } else if (func3 == 0x5){
        if(func7 == 0){ //srl
            set_register(rd, (uint32_t)get_register(rs1) >> (get_register(rs2) & 0x1F));
        } else if (func7 == 0x20){ //sra
            set_register(rd, get_register(rs1) >> (get_register(rs2) & 0x1F));
        } 
    }
    return 1;
}

int handle_i(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t func3, int two_previous){
    int32_t imm = (instruction >> 20) & 0xFFF;
    
    if(func3 == 0x6){ //uaddi
        if(two_previous == 1){ //2つ前の命令がauipc
            // printf("uaddi\n");
            imm = imm/4; //la
        }
        // printf("imm:%d\n",imm);
        handle_addi_case(rd,rs1,imm);
        // printf("rd(%d):%d\n",rd,get_register(rd));
    } else {
        //負の計算
        int minus = (imm & 0x800) ? 1 : 0;
        if (minus) {
            imm = ~imm & ((1 << 12) - 1);
            imm = - (imm + 1);
        }
        // 各種命令の処理
        if (func3 == 0) {  // addi命令
            handle_addi_case(rd, rs1, imm);
        } else if (func3 == 0x7) {  // andi命令
            set_register(rd, get_register(rs1) & imm);
        } else if (func3 == 0x6) {  // ori命令
            set_register(rd, get_register(rs1) | imm);
        } else if (func3 == 0x4) {  // xori命令
            set_register(rd, get_register(rs1) ^ imm);
        } else if (func3 == 0x1) {  // slli命令
            set_register(rd, get_register(rs1) << imm);
        }
    }
    return 1;
}

int handle_sw(uint32_t instruction, uint32_t rs1, uint32_t rs2, int current_line, FILE* memory_file, long long int total_count){
    // printf("sw\n");
    // counter.s_type[0]++;
    uint32_t sw_offset_11_5 = (instruction >> 25) & 0x8F;
    uint32_t sw_offset_4_0 = (instruction >> 4) & 0x1F;
    uint32_t imm = 0;

    imm |= (sw_offset_11_5 << 5);
    imm |= (sw_offset_4_0);
    if(imm && 0x800 == 1){//負の値
        uint32_t mask = (1<<12) - 1;
        imm = ~imm & mask + 1;
        imm = -imm;
    }
    if (0 <= rs2 && rs2 < 32) {
        memory[get_register(rs1) + imm] = get_register(rs2);
        // write_to_buffer(memory_file, &memory_buffer, 
        //                 "%d命令目 %d行目 memory%dの中に%dが格納される\n",
        //                 total_count, current_line+1, get_register(rs1)+imm, get_register(rs2));
    } else {
        memory[get_register(rs1) + imm] = get_float_register(rs2);
        // write_to_buffer(memory_file, &memory_buffer, 
        //                 "%d命令目 %d行目 memory%dの中に%lfが格納される\n",
        //                 total_count, current_line+1, get_register(rs1)+imm, get_float_register(rs2));
    }
    return 1;
}

int handle_b(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t func3){
    
    // printf("b_type\n");
    int pc = 0;
    uint32_t funct3 = (instruction >> 10) & 0x7;
    uint32_t bit12 = (instruction >> 31) & 0x1;
    uint32_t bit10_5 = (instruction >> 25) & 0x3F;
    uint32_t bit4_1 = (instruction >> 5) & 0xF;
    uint32_t bit11 = (instruction >> 4) & 0x1;
    uint32_t imm = 0;

    imm |= (bit12 << 12);
    imm |= (bit11 << 11);
    imm |= (bit10_5 << 5);
    imm |= (bit4_1 << 1);
    if(bit12 == 1){
        //immは負の値
        //2の補数
        uint32_t mask = (1 << 12) -1;
        imm = ~imm & mask;
        imm = - (imm + 1);
    }
    if (funct3 == 0) {  // beq
        //printf("beq\n");
        if(get_register(rs1) == get_register(rs2)){
            pc += imm/4;
        }
    } else if(funct3 == 0x1){  // bne
        if(get_register(rs1) != get_register(rs2)){
            pc += imm/4;
        }
    } else if(funct3 == 0x4){  // blt
        if(get_register(rs1) < get_register(rs2)){
            pc += imm/4;
        }
    } else if(funct3 == 0x5){  // bge
        if(get_register(rs1) >= get_register(rs2)){
            pc += imm/4;
        }
    } else if(funct3 == 0x6){  // bgt
        if(get_register(rs1) > get_register(rs2)){
            pc += imm/4;
        }
    }
    return pc;
}

int handle_lui(uint32_t instruction, int32_t rd){
    //lui rd 0xa だったらrdにa * 16 * 16 * 16を格納
    // printf("u_type\n");
    int32_t value = (((instruction >> 12) & 0xFFFFF) << 12); //これ4で割らないといけないかもしれない。4で割るなら行数と一致する
    set_register(rd,value);
    // counter.u_type[0]++;
    return 1;
}

int handle_auipc(uint32_t instruction, uint32_t rd, int current_line){
    // printf("auipc\n");
    uint32_t bit31_12 = (instruction >> 12) & 0xFFFFF;
    // 符号拡張を行う
    if (bit31_12 & 0x80000) {
        // 負の値の場合
        bit31_12 |= 0xFFF00000; // 上位ビットを1で埋める
    }
    int32_t value = (int32_t)(bit31_12 << 12) / 4 + current_line; // 符号付きの32ビット整数としてシフト
    set_register(rd,value);
    // printf("value:%d\n",value);
    return 1;
}

int handle_j(uint32_t instruction, uint32_t rd, int current_line){
    uint32_t bit20 = (instruction >> 31) & 0x1;
    uint32_t bit10_1 = (instruction >> 21) & 0x3FF;
    uint32_t bit11 = (instruction >> 20) & 0x1;
    uint32_t bit19_12 = (instruction >> 12) & 0xFF;
    int32_t imm = 0;
    imm |= (bit20 << 20);
    imm |= (bit19_12 << 12);
    imm |= (bit11 << 11);
    imm |= (bit10_1 << 1);
    //printf("Executed jal: x%d = PC + 1, PC += %d\n", rd, imm);
    // 符号拡張
    if (bit20) {
        imm |= 0xFFE00000;  // 負の値の場合、上位ビットを1で埋める
    }
    set_register(rd, current_line + 2);
    //printf("戻りアドレスrd: %d\n",get_register(rd));

    return imm/4;
}

int handle_jalr(uint32_t instruction, uint32_t rd, uint32_t rs1, int current_line){
    // printf("i_type (jalr)\n");
    // counter.jalr_type[0]++;
    int32_t imm = (instruction >> 20) & 0xFFF;
    int pc = 0;
    if(imm & 0x800){
        //即値が負の値
        uint32_t mask = (1 << 12) - 1;
        imm = -((~imm & mask) + 1);//反転
    }

    set_register(rd, current_line+2);
    pc = get_register(rs1) + imm/4 - current_line - 1;

    return pc;
}

int handle_lw(uint32_t instruction, uint32_t rd, uint32_t rs1, int current_line, FILE* memory_file){
    // printf("lw\n");
    uint32_t lw_offset = (instruction >> 20) & 0xFFF;
    float lw = 0; //setする値

    if(lw_offset && 0x800 == 1){//負の値
        uint32_t mask = (1<<12) - 1;
        lw_offset = ~lw_offset & mask;
        lw_offset = lw_offset + 1;
        lw = memory[get_register(rs1) - lw_offset];
    }else{
        //printf("正\n");
        lw = memory[get_register(rs1) + lw_offset];
    }
    //printf("memory%dの中に格納されている値:%f\n",get_register(rs1) + lw_offset,lw);
    // fprintf(memory_file,"%d行目 memory%dの中に格納されている値:%f\n",current_line+1, get_register(rs1) + lw_offset,lw);
    set_register(rd,lw);
    return 1;
}

int handle_f(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t func3){
    // printf("f_type\n");
    // pc_operand.pc = 1;
    //func: 0:fadd, 1:fsub, 2:fmul, 3:fdiv, 12:fabs, 13:fneg, 10:finv, 11: fsqrt, 4:fsgnjn/fsgnjx, 20: feq/flt, 24: fcvtws, 26: fcvtsw, 28: floor
    //printf("f形式");
    uint32_t func7 = (instruction >> 27) & 0x1F;
    float a1 = get_float_register(rs1);
    float a2 = get_float_register(rs2);
    //printf("a1:%f\n",a1);
    float result;
    if(func7 == 0){
        result = fadd(a1,a2);
        //printf("result:%f\n",result);
        //printf("fadd x%d x%d\n",r1,r2);
        //printf("result %f + %f = %f\n",a1,a2,result);
        set_register(rd, result);
        // counter.f_type[0]++;
    }
    if(func7 == 1){
        result = fsub(a1,a2);
        //printf("fsub x%d = x%d - x%d\n",rd, r1,r2);
        //printf("result %f - %f = %f\n",a1,a2,result);
        set_register(rd, result);
        // counter.f_type[1]++;
    }
    if(func7 == 2){
        result = fmul(a1,a2);
        //printf("fmul x%d = x%d * x%d\n",rd, r1,r2);
        //printf("result %f * %f = %f\n",a1,a2,result);
        set_register(rd, result);
        // counter.f_type[2]++;
    }
    if(func7 == 3){
        result = fdiv(a1,a2);
        set_register(rd, result);
        // counter.f_type[3]++;
    }
    if(func7 == 4){
        if(func3 == 1){// fsgnjn
            result = fsgnjn(a1,a2);
            set_register(rd, result);
            // counter.f_type[8]++;
        }
        if(func3 == 2){// fsgnjx
            result = fsgnjx(a1,a2);
            set_register(rd, result);
            // counter.f_type[9]++;
        }
    }
    if(func7 == 12){
        result = fabsf(a1);
        set_register(rd, result);
        // counter.f_type[4]++;
    }
    if(func7 == 13){
        result = fneg(a1);
        set_register(rd, result);
        // counter.f_type[5]++;
    }
    if(func7 == 10){
        result = finv(a1);
        set_register(rd, result);
        // counter.f_type[6]++;
    }
    if(func7 == 11){
        result = fsqrt(a1);
        set_register(rd, result);
        // counter.f_type[7]++;
    }
    if(func7 == 20){
        if(func3 == 1){//flt
            //printf("flt x%d, x%d, x%d\n", rd, r1, r2);
            bool comparison_result = flt(a1, a2);
            result = (double)comparison_result; // boolをdoubleに変換
            //printf("result: %f\n", result); // %fを使用してdoubleを表示
            set_register(rd, result);
            // counter.f_type[11]++;
        }
        if(func3 == 2){//feq
            bool comparison_result = feq(a1, a2);
            result = (double)comparison_result; // boolをdoubleに変換
            //printf("result: %f\n", result); // %fを使用してdoubleを表示
            set_register(rd, result);
            // counter.f_type[10]++;
        }
    }
    if(func7 == 24){
        int32_t int_result = fcvtws(a1); // int32_t 型で結果を受け取る
        result = (double)int_result;     // double 型にキャスト
        //printf("result: %f\n", result);  // %f を使用して double を表示
        set_register(rd, result);
        // counter.f_type[12]++;
    }
    if(func7 == 26){
        result = fcvtsw(get_register(rs1));
        set_register(rd, result);
        // counter.f_type[13]++;
    }
    if(func7 == 28){
        result = floor(a1);
        set_register(rd, result);
        // counter.f_type[14]++;
    }

    // rdにresultを格納
    set_register(rd, result);
    return 1;
}

int handle_c(uint32_t instruction, uint32_t rd, uint32_t func3, FILE* sld_file, FILE* sld_result_file){
    // printf("csrr/csrw\n");
    rd = (instruction >> 19) & 0x3F;
    //csrw_int
    if(func3 == 4){
        uint32_t value = (uint32_t)registers[rd]; 
        // printf("value:%d\n",value);
        //下位2bitで出力回数
        int total_output = (value & 0x3) + 1;
        for(int i=0; i <(3-total_output); i++){
            fprintf(sld_result_file, "0");
            // printf("0");
        }
        for(int i=0; i < total_output; i++){
            int shift_count = 2+i*8;
            uint8_t lower8bits = ((value >> shift_count) & 0xF);
            fprintf(sld_result_file, "%u", lower8bits);
            // printf("%u",lower8bits);
        }
        // fprintf(sld_result_file, "\n");
        // printf("\n");
    }
    //csrw
    if(func3 == 1){ // x10の下位8bit値をファイルに書きこむ
        if( 0 <= rd && rd < 32){
            uint32_t value = (uint32_t)get_register(rd);
            uint8_t lower8bits = (value & 0xFF);
            // uint8_t lower8bits = (value & 0xFF);
            // lower8bits = (lower8bits >= 48) ? lower8bits-48 : lower8bits;
            fprintf(sld_result_file, "%c", lower8bits);
            // printf("%c",lower8bits);
        } else {
            uint32_t value = (uint32_t)get_float_register(rd);
            // uint8_t lower8bits = (value & 0xFF) - 48;
            uint8_t lower8bits = (value & 0xFF);
            // lower8bits = (lower8bits >= 48) ? lower8bits-48 : lower8bits;
            fprintf(sld_result_file, "%c", lower8bits);
            // printf("%c",lower8bits);
        }
    }
    //csrr
    if(func3 == 2){ // rdにsldファイルの内容を書きこむ
        rd = (instruction >> 4) & 0x3F;
        int is_float;
        int32_t value = read_next_value_from_file(sld_file, &is_float);
        // printf("value:%d\n",value);
        if (is_float) {
            float float_value;
            memcpy(&float_value, &value, sizeof(float));
            set_register(rd, float_value);
            // printf("Float value: %f\n", float_value);
        } else {
            set_register(rd, value);
            // printf("Integer value: %d\n", (int32_t)value);
        }
    }
    return 1;
}

uint32_t previous_instruction = 0;
int current_line = 0;
// バイナリ命令をデコードして処理
int fast_execute_binary_instruction(BinaryInstruction binary_instruction[], int instruction_length, FILE* transition_file, FILE* float_transition_file, FILE* sld_file, FILE* sld_result_file, FILE* memory_file) {
    fflush(memory_file);
    initialize_finv_table();
    // オペコードを取得
    //下4桁
    long long int total_count = 0;
    int previous = 0;
    int two_previous = 0;
    uint32_t instruction, opcode, rd, rs1, rs2, func3;

    while(current_line < instruction_length){
        // printf("current_line:%d\n",current_line);
        // printf("total_count:%lld\n",total_count);
        total_count++;
        int pc = 0;
        two_previous = previous;
        previous = (opcode == 0x6);
        // printf("instruction:%s\n",binary_instruction[current_line].binary_code);
        instruction = strtol(binary_instruction[current_line].binary_code, NULL, 2); //2進数文字列を数値に変換
        opcode = instruction & 0xF;
        rd = (instruction >> 4) & 0x3F;
        rs1 = (instruction >> 13) & 0x3F;
        rs2 = (instruction >> 19) & 0x3F;
        func3 = (instruction >> 10) & 0x7;
        
        switch (opcode) {
            case 0x2:  // I-type
                handle_i(instruction, rd, rs1, func3, two_previous);
                break;
            case 0x3:  // SW
                handle_sw(instruction, rs1, rs2, current_line, memory_file, total_count);
                break;
            case 0xa:  // F-type
                handle_f(instruction, rd, rs1, rs2, func3);
                break;
            case 0x9:  // LW
                handle_lw(instruction, rd, rs1, current_line, memory_file);
                break;
            case 0x7:  // J-type
                pc = handle_j(instruction, rd, current_line);
                break;
            case 0x1:  // R-type
                handle_r(instruction, rd, rs1, rs2, func3);
                break;
            case 0x4:  // B-type
                pc = handle_b(instruction, rs1, rs2, func3);
                break;
            case 0x5:  // LUI
                handle_lui(instruction, rd);
                break;
            case 0x6:  // AUIPC
                handle_auipc(instruction, rd, current_line);
                break;
            case 0x8:  // JALR
                pc = handle_jalr(instruction, rd, rs1, current_line);
                break;
            case 0xb:  // CSR
                handle_c(instruction, rd, func3, sld_file, sld_result_file);
                break;
            case 0xe: //break
                printf("\nBreak point reached at instruction %lld. Press Enter to continue...\n", total_count);
                while(getchar() != '\n'); // Enterキーが押されるまで待機
                break;
            case 0xf:  // Finish
                printf("Finish instruction detected. Total instructions: %lld\n", total_count);
                return 1;
            default:
                break;
        }

        print_use_register_transition(transition_file,current_line+1,use_register);
        print_use_float_register_transition(float_transition_file,current_line+1,use_register);
        current_line += (pc == 0) ? 1 : pc;
        // printf("current_line:%d\n",current_line);
    }
    flush_buffer(memory_file, &memory_buffer);
    return 0;
}


void print_register(FILE* output_file){
    for(int i=0;i<32;i++){
        fprintf(output_file, "x%d = %d\n", i, get_register(i));
    }
}

void for_markdown(FILE *transition_file, FILE *float_transition_file, int use_regiser[64]){
    // Markdownの表ヘッダーを出力
    fprintf(transition_file, "| ");
    fprintf(transition_file, "実行命令|");
    fprintf(float_transition_file, "| ");
    fprintf(float_transition_file, "実行命令|");
    int int_count = 0;
    int float_count = 0;
    for (int i = 0; i < 32; i++) {
        if(use_register[i] > 0){
            int_count++;
            fprintf(transition_file, "x%-2d | ", i);
        }
    }    
    for (int i = 32; i < 64; i++) {
        if(use_register[i] > 0){
            float_count++;
            fprintf(float_transition_file, "f%-2d | ", i-32);
        }
    }
    fprintf(transition_file, "\n|");
    fprintf(float_transition_file, "\n|");

    // 区切り線を出力
    for (int i = 0; i < int_count+1; i++) {
        fprintf(transition_file, "---:|");
    }    
    for (int i = 0; i < float_count+1; i++) {
        fprintf(float_transition_file, "---:|");
    }
    fprintf(transition_file, "\n");
    fflush(transition_file);
    fprintf(float_transition_file, "\n");
    fflush(float_transition_file);
}

int instruction_count = 0;
int main(){

    //ファイルからassembly取得
    FILE *file;
    char assembly_code[MAX_ASSEMBLY_SIZE];
    char assembly_instructions[MAX_INSTRUCTIONS][MAX_INSTRUCTION_LENGTH];
    file = fopen("./document/assembly.txt","r");
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
    // printf("assmbly_code:%s\n",assembly_code);

    //assmblyをbinaryに変換
    remove_comments(assembly_code);
    // printf("assmbly_code:%s\n",assembly_code);

    // labelとその位置を記録
    found_labels(assembly_code);
    parse_assembly(assembly_code);

    char* token = strtok((char*)assembly_code, "\n");
    while (token != NULL && instruction_count < MAX_INSTRUCTIONS) {
        strncpy(assembly_instructions[instruction_count], token, MAX_INSTRUCTION_LENGTH - 1);
        assembly_instructions[instruction_count][MAX_INSTRUCTION_LENGTH - 1] = '\0'; // null終端を保証
        instruction_count++;
        token = strtok(NULL, "\n");
    }
    // printf("count:%d\n",instruction_count);

    //binary codeはbinary.txtにoutput
    FILE *output_file = fopen("./document/binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int instruction_length = print_binary_instructions(output_file) + 1;
    //printf("%d\n",instruction_length);
    fclose(output_file);
    // printf("length:%d\n",instruction_length);
    int assembly_count =  instruction_count - instruction_length;
    //printf("assembly_code:%20s",assembly_instructions[assembly_count]);

    //register遷移
    FILE *transition_file = fopen("./document/transition.md", "w");
    if (transition_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    // for_markdown(transition_file, use_register);

    FILE *float_transition_file = fopen("./document/float_transition.md", "w");
    if (float_transition_file == NULL) {
        perror("Error opening float_transition file");
        return 1;
    }

    for_markdown(transition_file, float_transition_file, use_register);

    //pipeline
    //binary codeを受け取ってpipelineにする
    FILE *pipeline_file = fopen("./document/pipeline.txt","w");
    if (pipeline_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    FILE *sld_file = fopen("./document/formatted_sld_data.txt","r");
    if (sld_file == NULL) {
        perror("Error opening sld file");
        return 1;
    }

    //memoryの値の遷移を表示
    FILE *memory_file = fopen("./document/memory_transition.txt","w");
    if (memory_file == NULL) {
        perror("Error opening memory file");
        return 1;
    }

    //x10の値が格納
    FILE *sld_result_file = fopen("./document/sld_result.txt","w");
    if (sld_result_file == NULL) {
        perror("Error opening sld_result file");
        return 1;
    }
    
    clock_t start_time, end_time;
    start_time = clock();

    fast_execute_binary_instruction(binary_instructions, instruction_length, transition_file, float_transition_file, sld_file, sld_result_file, memory_file);
    
    fclose(transition_file);
    fclose(sld_file);

    end_time = clock();

    printf("Execution Time = %lf [s]\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}