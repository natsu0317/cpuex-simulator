// binary から 実行結果を出力
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../float/math/math_functions.h"
#include "../asm_to_binary/asm_to_binary.h"
#include "binary_to_result.h"

#define NUM_REGISTERS 64
#define INSTRUCTION_LENGTH 33 //32bit + 終端文字
#define MEMORY_SIZE 8388608
#define STACK_SIZE 4096

float memory[MEMORY_SIZE];
int sp = MEMORY_SIZE - 1; //スタックポインタ

// registerのsimulation
double registers[NUM_REGISTERS] = {0};  // 64個のレジスタを初期化 0-31は整数　32-63はfloat

// typedef struct{
//     int pc;
//     int opcode; //sw,lwなら1,分岐命令なら2,それ以外なら0
//     int operand1;
//     int operand2;
//     int operand3;
// }Pc_operand;
extern InstructionCounter counter;

// レジスタの値を設定するreg_numは0 ~ 63
void set_register(int reg_num, double value) {
    // //printf("reg_num:%d\n",reg_num);
    // printf("value:%lf\n",value);
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < NUM_REGISTERS) {
        registers[reg_num] = value;
        //printf("reg_num:x%d = %lf\n",reg_num, value);
    }
}

// レジスタの値を取得する reg_num = 0~31(int version)
int get_register(int reg_num) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < 32) {
        return registers[reg_num];
    }
    //printf("reg[%d] = %d\n",reg_num,registers[reg_num]);
    return 0;
}
//reg_num 32 ~ 63
double get_float_register(int reg_num) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 32 && reg_num < NUM_REGISTERS) {
        return registers[reg_num];
    }
    //printf("reg[%d] = %f\n",reg_num,registers[reg_num]);
    return 0;
}

uint32_t read_next_value_from_file(FILE *file) {
    char line[50];
    if (fgets(line, sizeof(line), file)) {
        return (uint32_t)atoi(line); // 行を数値に変換して返す
    }
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
        counter.i_type[0]++;
    } else {
        handle_float_register(rd, rs1, imm);
    }
}

uint32_t previous_instruction = 0;
// バイナリ命令をデコードして処理
Pc_operand execute_binary_instruction(const char binary_instruction[][33], const char previous_binary_instruction[][33],const char two_previous_binary_instruction[][33], int num_instructions, int current_line, FILE* sld_file, FILE* sld_result_file, FILE* memory_file) {
    Pc_operand pc_operand;
    pc_operand.opcode = 0;
    pc_operand.pc = 1;
    //printf("current_line:%d\n",current_line);
    fflush(memory_file);
    for(int pc=0; pc<num_instructions; pc++){

        //printf("x1:%d\n",get_register(1));
        //printf("instruction :%s\n",binary_instruction[pc]);
        // printf("previous_instruction :%s\n",previous_binary_instruction[pc]);
        // printf("two_previous_instruction :%s\n",two_previous_binary_instruction[pc]);
        uint32_t instruction = strtol(binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換
        uint32_t previous_instruction = strtol(previous_binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換
        uint32_t two_previous_instruction = strtol(two_previous_binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換

        // オペコードを取得
        //下4桁
        uint32_t opcode = instruction & 0xF;
        uint32_t previous_opcode = previous_instruction & 0xF;
        uint32_t two_previous_opcode = two_previous_instruction & 0xF;
        //printf("opcode:%x\n",opcode);

        switch (opcode) {
            case 0x0:   //label部分
                {
                    pc_operand.pc = 1;
                }
                return pc_operand;
            case 0x1:  // R形式命令 (例:"add", "sub", "and", "or", "xor",)
                {
                    // printf("r_type\n");
                    uint32_t funct3 = (instruction >>10) & 0x7;
                    uint32_t funct7 = (instruction >> 25) & 0x7F;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rs2 = (instruction >> 19) & 0x3F;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                    pc_operand.operand3 = rs2;
                    double rs1_value;
                    double rs2_value;
                    if(0 <= rs1 & rs1 < 32){
                        //整数レジスタ
                        rs1_value = get_register(rs1);
                    } else {
                        rs1_value = get_float_register(rs1);
                    }
                    if(0 <= rs2 & rs2 < 32){
                        //整数レジスタ
                        rs2_value = get_register(rs2);
                    } else {
                        rs2_value = get_float_register(rs2);
                    }

                    if (funct3 == 0 && funct7 == 0) {  // add命令
                        set_register(rd, rs1_value + rs2_value);
                        counter.r_type[0]++;
                    } else if (funct3 == 0 && funct7 == 0x20) {  // sub命令
                        set_register(rd, rs1_value - rs2_value);
                        counter.r_type[1]++;
                    } else if (funct3 == 0x7){  // and
                        set_register(rd, get_register(rs1) & get_register(rs2));
                        counter.r_type[2]++;
                    } else if (funct3 == 0x4){  // xor
                        set_register(rd, get_register(rs1) ^ get_register(rs2));
                        counter.r_type[4]++;
                    }  else if (funct3 == 0x3){ //div10(商)
                        // // printf("rs1_value: %u\n", rs1_value);
                        // uint32_t quotient = (uint32_t)rs1_value / 10;
                        // uint32_t remainder = (uint32_t)rs1_value % 10;
                        // // printf("商: %u, 余り: %u\n", quotient, remainder);
                        // uint32_t result = (quotient << 8) | (remainder & 0xFF);
                        // set_register(rd, result);
                        uint32_t digits[4] = {0};
                        int num_digits = 0;
                        // 10で割って各桁を取得
                        while((int)rs1_value > 0 && num_digits < 4){
                            digits[num_digits++] = (uint32_t)rs1_value % 10;
                            rs1_value /= 10;
                        }
                        uint32_t result = 0;
                        for(int i=0; i<num_digits; i++){
                            result |= ((digits[i] + '0') << ((3-i)*8));
                        }
                        if(num_digits < 4){
                            result >>= (4 - num_digits) * 4 + 2;
                        } else {
                            result <<= 2;
                        }
                        result |= ((num_digits - 1) & 0x3);
                        // result <<= (4 - num_digits) * 8;
                        set_register(rd, result);
                        counter.r_type[5]++;
                    } else if (funct3 == 0x6){ //rem(余り)
                        set_register(rd, get_register(rs1) % get_register(rs2));
                    } else if (funct3 == 0x1){ //shift
                        set_register(rd, get_register(rs1) << (get_register(rs2) & 0x1F));
                        counter.r_type[6]++;
                    } else if (funct3 == 0x5 && funct7 == 0){ //srl
                        set_register(rd, (uint32_t)get_register(rs1) >> (get_register(rs2) & 0x1F));
                        counter.r_type[7]++;
                    } else if (funct3 == 0x5 && funct7 == 0x20){ //sra
                        set_register(rd, get_register(rs1) >> (get_register(rs2) & 0x1F));
                        counter.r_type[8]++;
                    }
                }
                return pc_operand;

            case 0x2:  // I形式命令 (例: "addi", "andi", "ori", "xori")
                {   
                   //printf("i_type\n");
                    uint32_t funct3 = (instruction >> 10) & 0x7;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    int32_t imm = (instruction >> 20) & 0xFFF;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                    
                    if(funct3 == 0x6){ //uaddi
                        if(previous_opcode == 0x2){
                            imm = imm/4; //la
                        }
                        handle_addi_case(rd,rs1,imm);
                    } else {
                        //負の計算
                        int minus = (imm & 0x800) ? 1 : 0;
                        if (minus) {
                            imm = ~imm & ((1 << 12) - 1);
                            imm += 1;
                        }
                        // 各種命令の処理
                        if (funct3 == 0) {  // addi命令
                            if (two_previous_opcode == 0x5) {
                                imm /= 4;
                            }
                            if(minus) {
                                imm = -imm;
                            }
                            handle_addi_case(rd, rs1, imm);
                            counter.i_type[0]++;
                        } else if (funct3 == 0x7) {  // andi命令
                            set_register(rd, get_register(rs1) & imm);
                            counter.i_type[1]++;
                        } else if (funct3 == 0x6) {  // ori命令
                            set_register(rd, get_register(rs1) | imm);
                            counter.i_type[2]++;
                        } else if (funct3 == 0x4) {  // xori命令
                            set_register(rd, get_register(rs1) ^ imm);
                            counter.i_type[3]++;
                        } else if (funct3 == 0x1) {  // slli命令
                            set_register(rd, get_register(rs1) << imm);
                            counter.i_type[4]++;
                        }
                    }
                }
                return pc_operand;

            case 0x3:{   // sw mem[x[r1] + offset] = x[r2]
                    //printf("sw\n");
                    counter.s_type[0]++;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rs2 = (instruction >> 19) & 0x3F;
                    uint32_t sw_offset_11_5 = (instruction >> 25) & 0x8F;
                    uint32_t sw_offset_4_0 = (instruction >> 4) & 0x1F;
                    uint32_t imm = 0;
                    pc_operand.opcode = 1;
                    pc_operand.operand2 = rs1;
                    pc_operand.operand3 = rs2;
                    imm |= (sw_offset_11_5 << 5);
                    imm |= (sw_offset_4_0);
                    if(imm && 0x800 == 1){//負の値
                        uint32_t mask = (1<<12) - 1;
                        imm = ~imm & mask + 1;
                        imm = -imm;
                    }
                    if( 0 <= rs2 && rs2 < 32 ){
                        memory[get_register(rs1) + imm] = get_register(rs2);
                        //printf("sw: x%d, %d(x%d)\n", rs2, imm, rs1);
                        //printf("memory%dの中に%dが格納される\n",get_register(rs1)+imm,get_register(rs2));
                        fprintf(memory_file,"%d行目 memory%dの中に%dが格納される\n",current_line+1, get_register(rs1)+imm, get_register(rs2));
                    } else {
                        memory[get_register(rs1) + imm] = get_float_register(rs2);
                        //printf("sw: x%d, %d(x%d)\n", rs2, imm, rs1);
                        //printf("memory%dの中に%lfが格納される\n",get_register(rs1)+imm,get_float_register(rs2));
                        fprintf(memory_file,"%d行目 memory%dの中に%lfが格納される\n",current_line+1, get_register(rs1)+imm, get_float_register(rs2));
                    } 
                    counter.s_type[0]++;
                } 
                return pc_operand;

            case 0x4:  // B形式命令 (例: "beq", "bne", "blt", "bge", "bgt")
                {
                    //printf("b_type\n");
                    uint32_t funct3 = (instruction >> 10) & 0x7;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rs2 = (instruction >> 19) & 0x3F;
                    uint32_t bit12 = (instruction >> 31) & 0x1;
                    uint32_t bit10_5 = (instruction >> 25) & 0x3F;
                    uint32_t bit4_1 = (instruction >> 5) & 0xF;
                    uint32_t bit11 = (instruction >> 4) & 0x1;
                    uint32_t imm = 0;
                    pc_operand.opcode = 2; 
                    pc_operand.operand2 = rs1;
                    pc_operand.operand3 = rs2;
                    imm |= (bit12 << 12);
                    imm |= (bit11 << 11);
                    imm |= (bit10_5 << 5);
                    imm |= (bit4_1 << 1);
                    if(bit12 == 1){
                        //immは負の値
                        //2の補数
                        uint32_t mask = (1 << 12) -1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                        if (funct3 == 0) {  // beq
                            if(get_register(rs1) == get_register(rs2)){
                               //printf("beq: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[0]++;
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x1){  // bne
                            if(get_register(rs1) != get_register(rs2)){
                               //printf("bne: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[1]++;
                               //printf("pc:%d\n",pc);
                                pc -= imm/4;
                               //printf("after_pc:%d\n",pc);
                            }
                        } else if(funct3 == 0x4){  // blt
                            if(get_register(rs1) < get_register(rs2)){
                               //printf("blt: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[2]++;
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x5){  // bge
                            if(get_register(rs1) >= get_register(rs2)){
                               //printf("bge: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[3]++;
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x6){  // bgt
                            if(get_register(rs1) > get_register(rs2)){
                               //printf("bgt: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[4]++;
                                pc -= imm/4;
                            }
                        }  else {
                            pc = 1;
                        }
                    } else if (funct3 == 0) {  // beq
                       //printf("beq\n");
                        if(get_register(rs1) == get_register(rs2)){
                           //printf("beq: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[0]++;
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x1){  // bne
                        if(get_register(rs1) != get_register(rs2)){
                           //printf("bne: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[1]++;
                            //printf("pc:%d\n",pc);
                            pc += imm/4;
                            //printf("after_pc:%d\n",pc);
                        }
                    } else if(funct3 == 0x4){  // blt
                        if(get_register(rs1) < get_register(rs2)){
                           //printf("blt: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[2]++;
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x5){  // bge
                        //printf("bge");
                        //printf("rs1: %d, rs2: %d\n",get_register(rs1),get_register(rs2));
                        counter.b_type[3]++;
                        if(get_register(rs1) >= get_register(rs2)){
                           //printf("bge: x%d, x%d, %d\n", rs1, rs2, imm);
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x6){  // bgt
                        //printf("bgt");
                        //printf("rs1: %d, rs2: %d\n",get_register(rs1),get_register(rs2));
                        counter.b_type[4]++;
                        if(get_register(rs1) > get_register(rs2)){
                           //printf("bgt: x%d, x%d, %d\n", rs1, rs2, imm);
                            pc += imm/4;
                        }
                    }
                    if(pc == 0){
                        pc = 1;
                    }
                    pc_operand.pc = pc;
                }
                return pc_operand;

            case 0x5: //lui
                {
                    //lui rd 0xa だったらrdにa * 16 * 16 * 16を格納
                    // printf("u_type\n");
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t bit31_12 = (instruction >> 12) & 0xFFFFF;
                    uint32_t value = (bit31_12 << 12); //これ4で割らないといけないかもしれない。4で割るなら行数と一致する
                    // value = value / 4;
                    //printf("lui x%d, %d", rd, value);
                    set_register(rd,value);
                    counter.u_type[0]++;
                }
                return pc_operand;

            case 0x6: //auipc {upimm, 12'b0} + pc
                {
                    //printf("auipc\n");
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t bit31_12 = (instruction >> 12) & 0xFFFFF;
                    //printf("%x\n",bit31_12);
                    // uint32_t value = (bit31_12 << 12); //これも4で割るべきかも？ それか下のcurrent_lineを4倍する
                    // if(bit31_12 & 0x80000){
                    //     //負の値
                    //     uint32_t mask = 0xFFFFFFFF;
                    //     value = ~value & mask;
                    //     value = value + 1;
                    // }
                    // 符号拡張を行う
                    if (bit31_12 & 0x80000) {
                        // 負の値の場合
                        bit31_12 |= 0xFFF00000; // 上位ビットを1で埋める
                    }
                    int32_t value = (int32_t)(bit31_12 << 12); // 符号付きの32ビット整数としてシフト
                    //printf("value:%d\n",value);
                    value = value / 4;
                    value = value + current_line; 
                    set_register(rd,value);
                    counter.aui_type[0]++;
                    //printf("auipc x%d ,%d\n",rd,bit31_12);
                }
                return pc_operand;

            case 0x7:  // J形式命令 (例: "jal")
                {
                    //printf("jal\n");
                    //printf("%x\n",instruction);
                    uint32_t bit20 = (instruction >> 31) & 0x1;
                    uint32_t bit10_1 = (instruction >> 21) & 0x3FF;
                    uint32_t bit11 = (instruction >> 20) & 0x1;
                    uint32_t bit19_12 = (instruction >> 12) & 0xFF;
                    uint32_t rd = (instruction >> 4) & 0x3F;
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
                    int next_line = current_line + imm/4;
                    //printf("imm: %d\n",imm);
                    //printf("jal: x%d, %d\n", rd, imm);
                    //printf("jal x%d, %d (PC: %d -> %d)\n", rd, imm, current_line +1, next_line +1);
                    counter.j_type[0]++;
                    // 戻りアドレスを保存
                    //printf("戻りアドレスrd: %d\n",current_line+2);
                    set_register(rd, current_line + 2);
                    //printf("戻りアドレスrd: %d\n",get_register(rd));
                    
                    // PCの更新後に即座にreturn
                    pc = imm/4;
                    pc_operand.pc = pc;
                }
                return pc_operand;
                
            case 0x8:  // I形式命令 ("jalr")
                {   
                   //printf("i_type (jalr)\n");
                   counter.jalr_type[0]++;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    int32_t imm = (instruction >> 20) & 0xFFF;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                   //printf("imm:%x\n",imm);
                    int minus = 0;//immが負なら1
                    if(imm & 0x800){
                        //即値が負の値
                        uint32_t mask = (1 << 12) - 1;
                       //printf("%x\n",mask);
                        imm = ~imm & mask;//反転
                        imm = imm+1;
                        minus = 1;
                       //printf("minus%d\n",minus);
                    }
                   //printf("after_imm:%x\n",imm);
                    //printf("funct3:%x\n,opcode:%x\n",funct3,opcode);

                    if(minus == 0){//immは正
                        //printf("immが正");
                        //printf("jalr: x%d, x%d, %d\n", rd, rs1, imm);
                        set_register(rd,current_line+2);
                        pc = get_register(rs1) + imm/4 - current_line - 1;
                        // pc = get_register(rs1) + imm/4;
                        // set_register(rd, get_register(rs1) + imm/4);
                        //printf("rs1:%d\n",get_register(rs1));
                        //printf("pc:%d\n",pc);
                        //printf("rd:x%d = %d\n",rd,get_register(rd));
                    }else if(minus == 1){
                        //printf("jalr: x%d, x%d, -%d\n", rd, rs1, imm);
                        set_register(rd,current_line+2);
                        pc = get_register(rs1) - imm/4 - current_line - 1;
                        // set_register(rd, get_register(rs1) - imm/4);
                        //printf("rs1:%d\n",get_register(rs1));
                        //printf("pc:%d\n",pc);
                    }
                    //printf("x1 = %d\n",get_register(1));
                }
               //printf("pc:%d\n",pc);
                if(pc == 0){
                    pc = 1;
                }
                pc_operand.pc = pc;
                return pc_operand;

            case 0x9:  // lw  x[rd] = mem[x[r1] + offset]
                {
                   //printf("lw\n");
                   counter.lw_type[0]++;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t lw_offset = (instruction >> 20) & 0xFFF;
                    //printf("rs1:x%d,rd;x%d,lw_offset:%d\n",rs1,rd,lw_offset);
                    float lw = 0; //setする値
                    pc_operand.opcode = 1;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                    if(lw_offset && 0x800 == 1){//負の値
                        uint32_t mask = (1<<12) - 1;
                        lw_offset = ~lw_offset & mask;
                        lw_offset = lw_offset + 1;
                        lw = memory[get_register(rs1) - lw_offset];
                       //printf("lw: x%d, -%d(x%d)\n", rd, lw_offset, rs1);
                    }else{
                        //printf("正\n");
                        //printf("address:%d + %d\n",get_register(rs1),lw_offset);
                        lw = memory[get_register(rs1) + lw_offset];
                       //printf("lw: x%d, %d(x%d)\n", rd, lw_offset, rs1);
                    }
                    //printf("memory%dの中に格納されている値:%f\n",get_register(rs1) + lw_offset,lw);
                    fprintf(memory_file,"%d行目 memory%dの中に格納されている値:%f\n",current_line+1, get_register(rs1) + lw_offset,lw);
                    set_register(rd,lw);
                }   
                return pc_operand;
            
            case 0xa:   //fadd,fsub,fmul,fdiv
                {   
                    //printf("f_type\n");
                    pc_operand.pc = 1;
                    //func: 0:fadd, 1:fsub, 2:fmul, 3:fdiv, 12:fabs, 13:fneg, 10:finv, 11: fsqrt, 4:fsgnjn/fsgnjx, 20: feq/flt, 24: fcvtws, 26: fcvtsw
                    //printf("f形式");
                    uint32_t func7 = (instruction >> 27) & 0x1F;
                    uint32_t func3 = (instruction >> 10) & 0x7;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t r1 = (instruction >> 13) & 0x3F;
                    uint32_t r2 = (instruction >> 19) & 0x3F;
                    float a1 = get_float_register(r1);
                    float a2 = get_float_register(r2);
                    //printf("a1:%f\n",a1);
                    //printf("funct3: %x\n",func3);
                    float result;
                    if(func7 == 0){
                        result = fadd(a1,a2);
                        //printf("result:%f\n",result);
                        //printf("fadd x%d x%d\n",r1,r2);
                        //printf("result %f + %f = %f\n",a1,a2,result);
                        set_register(rd, result);
                        counter.f_type[0]++;
                    }
                    if(func7 == 1){
                        result = fsub(a1,a2);
                        //printf("fsub x%d = x%d - x%d\n",rd, r1,r2);
                        //printf("result %f - %f = %f\n",a1,a2,result);
                        set_register(rd, result);
                        counter.f_type[1]++;
                    }
                    if(func7 == 2){
                        result = fmul(a1,a2);
                        //printf("fmul x%d = x%d * x%d\n",rd, r1,r2);
                        //printf("result %f * %f = %f\n",a1,a2,result);
                        set_register(rd, result);
                        counter.f_type[2]++;
                    }
                    if(func7 == 3){
                        result = fdiv(a1,a2);
                        set_register(rd, result);
                        counter.f_type[3]++;
                    }
                    if(func7 == 4){
                        if(func3 == 1){// fsgnjn
                            result = fsgnjn(a1,a2);
                            set_register(rd, result);
                            counter.f_type[8]++;
                        }
                        if(func3 == 2){// fsgnjx
                            result = fsgnjx(a1,a2);
                            set_register(rd, result);
                            counter.f_type[9]++;
                        }
                    }
                    if(func7 == 12){
                        result = fabsf(a1);
                        set_register(rd, result);
                        counter.f_type[4]++;
                    }
                    if(func7 == 13){
                        result = fneg(a1);
                        set_register(rd, result);
                        counter.f_type[5]++;
                    }
                    if(func7 == 10){
                        result = finv(a1);
                        set_register(rd, result);
                        counter.f_type[6]++;
                    }
                    if(func7 == 11){
                        result = fsqrt(a1);
                        set_register(rd, result);
                        counter.f_type[7]++;
                    }
                    if(func7 == 20){
                        if(func3 == 1){//flt
                            //printf("flt x%d, x%d, x%d\n", rd, r1, r2);
                            bool comparison_result = flt(a1, a2);
                            result = (double)comparison_result; // boolをdoubleに変換
                            //printf("result: %f\n", result); // %fを使用してdoubleを表示
                            set_register(rd, result);
                            counter.f_type[11]++;
                        }
                        if(func3 == 2){//feq
                            bool comparison_result = feq(a1, a2);
                            result = (double)comparison_result; // boolをdoubleに変換
                            //printf("result: %f\n", result); // %fを使用してdoubleを表示
                            set_register(rd, result);
                            counter.f_type[10]++;
                        }
                    }
                    if(func7 == 24){
                        int32_t int_result = fcvtws(a1); // int32_t 型で結果を受け取る
                        result = (double)int_result;     // double 型にキャスト
                        //printf("result: %f\n", result);  // %f を使用して double を表示
                        set_register(rd, result);
                        counter.f_type[12]++;
                    }
                    if(func7 == 26){
                        result = fcvtsw(a1);
                        set_register(rd, result);
                        counter.f_type[13]++;
                    }

                    // rdにresultを格納
                    set_register(rd, result);
                }
                return pc_operand;
                
            case 0xb:  // csrr, csrw, csrw_int
                {
                    // printf("csrr/csrw\n");
                    uint32_t rd = (instruction >> 19) & 0x3F;
                    uint32_t func = (instruction >> 10) & 0x7;
                    //csrw_int
                    if(func == 4){
                        uint32_t value = (uint32_t)registers[rd]; 
                        //下位2bitで出力回数
                        int total_output = (value & 0x3) + 1;
                        for(int i=0; i < total_output; i++){
                            int shift_count = 2+i*8;
                            uint8_t lower8bits = (value >> shift_count) & 0xF;
                            fprintf(sld_result_file, "%u\n", lower8bits);
                        }
                    }
                    //csrw
                    if(func == 1){ // x10の下位8bit値をファイルに書きこむ
                        if( 0 <= rd && rd < 32){
                            uint32_t value = (uint32_t)get_register(rd);
                            uint8_t lower8bits = value & 0xFF;
                            // double value = get_register(rd);
                            //printf("x%dの中身%fの値をファイルに書き込む",rd,value);
                            // fprintf(sld_result_file, "%f\n", value);
                            fprintf(sld_result_file, "%u\n", lower8bits);
                            counter.c_type[1]++;
                        } else {
                            uint32_t value = (uint32_t)get_float_register(rd);
                            uint8_t lower8bits = value & 0xFF;
                            fprintf(sld_result_file, "%u\n", lower8bits);
                            // double value = get_float_register(rd);
                            // printf("f%dの中身%fの値をファイルに書き込む",rd-32,value);
                            // fprintf(sld_result_file, "%f\n", value);
                            counter.c_type[1]++;
                        }
                    }
                    //csrr
                    if(func == 2){ // x10にsldファイルの内容を書きこむ
                        //sldの中身はとりあえずdoubleと仮定
                        double value = read_next_value_from_file(sld_file);
                        set_register(10, value);
                        counter.c_type[0]++;
                    }
                }
                return pc_operand;
        }
    }

}

void print_register(FILE* output_file){
    for(int i=0;i<32;i++){
        fprintf(output_file, "x%d = %d\n", i, get_register(i));
    }
}

// void print_register_transition(FILE *transition_file, FILE *float_transition_file, int pc){
//     fprintf(transition_file, "| ");
//     fprintf(transition_file, "%2d行|",pc);    
//     fprintf(float_transition_file, "| ");
//     fprintf(float_transition_file, "%2d行|",pc);
//     for(int i = 0; i< 32; i++) {
//         fprintf(transition_file, "%3d | ", get_register(i));
//     }    
//     for(int i = 32; i< NUM_REGISTERS; i++) {
//         fprintf(float_transition_file, "%3f | ", get_float_register(i));
//     }
//     fprintf(transition_file, "\n");
//     fprintf(float_transition_file, "\n");
// }

void print_use_register_transition(FILE *transition_file, int pc, int use_register[64]){
    fprintf(transition_file, "| ");
    fprintf(transition_file, "%2d行|",pc);   
    fflush(transition_file); 
    for(int i = 0; i< 32; i++) {
        if(use_register[i] > 0){
            fprintf(transition_file, "%3d | ", get_register(i));
        }
        // fprintf(transition_file, "%3d | ", get_register(i));
    }    
    fprintf(transition_file, "\n");
}

void print_use_float_register_transition(FILE *float_transition_file, int pc, int use_register[64]){ 
    fprintf(float_transition_file, "| ");
    fprintf(float_transition_file, "%2d行|",pc);
    fflush(float_transition_file);
    //printf("current_line:%d\n",pc);
    for(int i = 32; i< NUM_REGISTERS; i++) {
        if(use_register[i] > 0){
            fprintf(float_transition_file, "%3f | ", get_float_register(i));
        }
        // fprintf(float_transition_file, "%3f | ", get_float_register(i));
    }
    fprintf(float_transition_file, "\n");
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

int result_main() {
    char binary_instructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH];  // 読み取った命令を格納する配列
    FILE *file = fopen("/home/natsu/cpuex-simulator/source/asm_to_binary/binary.txt", "r");
    if (file == NULL) {
       //printf("Error: Could not open file\n");
        return 1;
    }

    int index = 0;
    while (fgets(binary_instructions[index], INSTRUCTION_LENGTH, file) != NULL && index < MAX_INSTRUCTIONS) {
        // 改行文字を削除
        //printf("index:%d\n",index);
        binary_instructions[index][strcspn(binary_instructions[index], "\n")] = '\0';//最後の文字を\0に
        //printf("%s\n",binary_instructions[index]);
        if(strlen(binary_instructions[index]) == 0){
            continue;
        }
        //printf("%ld\n",strlen(binary_instructions[index]));
        index++;
        //printf("after_index:%d\n",index);
    }
   //printf("index:%d\n",index);
    int current_line = 0;
    char input[10];

    FILE *transition_file = fopen("transition.md", "w");
    if (transition_file == NULL) {
        perror("Error opening transition file");
        return 1;
    }

    // for_markdown(transition_file);

    while (current_line < index) {    
        int pc = 0;
        if(strcmp(binary_instructions[current_line],"11111111111111111111111111111111") == 0){
            break;
        }
        Pc_operand pc_opcode_operand1;
        // pc_opcode_operand1 = execute_binary_instruction(&binary_instructions[current_line], 1, current_line);
        pc = pc_opcode_operand1.pc;

        // print_register_transition(transition_file, float_transition_file, current_line);
        fflush(transition_file);
        
        if (pc == 1) {
            current_line++;
        } else {
            current_line += pc;
        }
    }
    
    fclose(transition_file);

    // レジスタの状態を別のファイルに出力
    FILE *output_file = fopen("result.txt", "w");
    if (output_file == NULL) {
        perror("Error opening result file");
        return 1;
    }
    print_register(output_file);
    fclose(output_file);

    return 0;
}
