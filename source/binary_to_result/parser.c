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

int memory[MEMORY_SIZE];
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
    printf("reg_num:%d\n",reg_num);
    printf("value:%lf\n",value);
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < NUM_REGISTERS) {
        registers[reg_num] = value;
        printf("reg_num:x%d = %lf\n",reg_num, value);
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
    //printf("reg[%d] = %d\n",reg_num,registers[reg_num]);
    return 0;
}

uint32_t read_next_value_from_file(FILE *file) {
    char line[50];
    if (fgets(line, sizeof(line), file)) {
        return (uint32_t)atoi(line); // 行を数値に変換して返す
    }
    return 0; // ファイルの終わりやエラーの場合
}

// バイナリ命令をデコードして処理
Pc_operand execute_binary_instruction(const char binary_instruction[][33], int num_instructions, int current_line, FILE* sld_file, FILE* sld_result_file, FILE* memory_file) {
    Pc_operand pc_operand;
    pc_operand.opcode = 0;
    pc_operand.pc = 1;
    printf("current_line:%d",current_line);
    for(int pc=0; pc<num_instructions; pc++){
        // pcは常に0

        //printf("x1:%d\n",get_register(1));
        printf("instruction :%s\n",binary_instruction[pc]);
        uint32_t instruction = strtol(binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換

        // オペコードを取得
        //下4桁
        uint32_t opcode = instruction & 0xF;
        printf("opcode:%x\n",opcode);

        switch (opcode) {
            case 0x0:   //label部分
                {
                    pc_operand.pc = 1;
                }
                return pc_operand;
            case 0x1:  // R形式命令 (例:"add", "sub", "and", "or", "xor",)
                {
                    printf("r_type\n");
                    uint32_t funct3 = (instruction >>10) & 0x7;
                    uint32_t funct7 = (instruction >> 25) & 0x7F;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rs2 = (instruction >> 19) & 0x3F;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                    pc_operand.operand3 = rs2;

                    if (funct3 == 0 && funct7 == 0) {  // add命令
                        set_register(rd, get_register(rs1) + get_register(rs2));
                        counter.r_type[0]++;
                       printf("add: x%d, x%d, x%d\n", rd, rs1, rs2);
                        //printf("result:%d\n",get_register(rd));
                    } else if (funct3 == 0 && funct7 == 0x20) {  // sub命令
                        set_register(rd, get_register(rs1) - get_register(rs2));
                        counter.r_type[1]++;
                       printf("sub: x%d, x%d, x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x7 && funct7 == 0){  // and
                        set_register(rd, get_register(rs1) & get_register(rs2));
                        counter.r_type[2]++;
                       printf("and: x%d, x%d, x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x6 && funct7 == 0){  // or
                        set_register(rd, get_register(rs1) | get_register(rs2));
                        counter.r_type[3]++;
                       printf("or: x%d, x%d, x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x4 && funct7 == 0){  // xor
                        set_register(rd, get_register(rs1) ^ get_register(rs2));
                        counter.r_type[4]++;
                       printf("xor: x%d, x%d, x%d\n", rd, rs1, rs2);
                    }  
                }
                return pc_operand;

            case 0x2:  // I形式命令 (例: "addi", "andi", "ori", "xori")
                {   
                   printf("i_type\n");
                    uint32_t funct3 = (instruction >> 10) & 0x7;
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
                    printf("funct3:%x\n,opcode:%x\n",funct3,opcode);

                    if(minus == 0){//immは正
                        if (funct3 == 0) {  // addi命令
                            set_register(rd, get_register(rs1) + imm);
                            counter.i_type[0]++;
                           printf("addi: x%d, x%d, %d\n", rd, rs1, imm);
                           //printf("result:%d\n",get_register(rd));
                        } else if (funct3 == 0x7){  // andi
                            set_register(rd, get_register(rs1) & imm);
                            counter.i_type[1]++;
                           printf("andi: x%d, x%d, %d\n", rd, rs1, imm);
                        } else if (funct3 == 0x6){  // ori
                            set_register(rd, get_register(rs1) | imm);
                            counter.i_type[2]++;
                           printf("andi: x%d, x%d, %d\n", rd, rs1, imm);
                        } else if (funct3 == 0x4){  // xori
                            set_register(rd, get_register(rs1) ^ imm);
                            counter.i_type[3]++;
                           printf("xori: x%d, x%d, %d\n", rd, rs1, imm);
                        } else if (funct3 == 0x1){ // rd = rs1 << imm
                            counter.i_type[4]++;
                            set_register(rd, get_register(rs1) << imm);
                           printf("slli: x%d, x%d, %d\n", rd, rs1, imm);
                        }
                    }else if(minus == 1){
                        if (funct3 == 0) {  // addi命令
                            set_register(rd, get_register(rs1) - imm);
                            counter.i_type[0]++;
                            printf("addi: x%d, x%d, -%d\n", rd, rs1, imm);
                            //printf("result:%d\n",get_register(rd));
                        }
                    }
                }
                return pc_operand;

            case 0x3:{   // sw mem[x[r1] + offset] = x[r2]
                    printf("sw\n");
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
                        imm = ~imm & mask;
                        imm = imm + 1;
                        memory[get_register(rs1) - imm] = get_register(rs2);
                       printf("sw: x%d, -%d(x%d)\n", rs2, imm, rs1);
                       printf("memory%dの中に%dが格納される\n",get_register(rs1)-imm,get_register(rs2));
                       fprintf(memory_file,"memory%dの中に%dが格納される\n",get_register(rs1)-imm,get_register(rs2));
                    }else{
                        memory[get_register(rs1) + imm] = get_register(rs2);
                       printf("sw: x%d, %d(x%d)\n", rs2, imm, rs1);
                       printf("memory%dの中に%dが格納される\n",get_register(rs1)+imm,get_register(rs2));
                       fprintf(memory_file,"memory%dの中に%dが格納される\n",get_register(rs1)+imm,get_register(rs2));
                    }    
                } 
                return pc_operand;

            case 0x4:  // B形式命令 (例: "beq", "bne", "blt", "bge")
                {
                    printf("b_type\n");
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
                   printf("12; %d, 11: %d, 10_5: %d, 4_1: %d\n",bit12,bit11,bit10_5,bit4_1);
                   printf("funct3:%d\n",funct3);
                    imm |= (bit12 << 12);
                    imm |= (bit11 << 11);
                    imm |= (bit10_5 << 5);
                    imm |= (bit4_1 << 1);
                   printf("imm:%d\n",imm);
                    if(bit12 == 1){
                        //immは負の値
                        //2の補数
                        uint32_t mask = (1 << 12) -1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                       //printf("imm:%d\n",imm);
                        if (funct3 == 0) {  // beq
                            if(get_register(rs1) == get_register(rs2)){
                               printf("beq: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[0]++;
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x1){  // bne
                            if(get_register(rs1) != get_register(rs2)){
                               printf("bne: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[1]++;
                               //printf("pc:%d\n",pc);
                                pc -= imm/4;
                               //printf("after_pc:%d\n",pc);
                            }
                        } else if(funct3 == 0x4){  // blt
                            if(get_register(rs1) < get_register(rs2)){
                               printf("blt: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[2]++;
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x5){  // bge
                            if(get_register(rs1) >= get_register(rs2)){
                               printf("bge: x%d, x%d, -%d\n", rs1, rs2, imm);
                               counter.b_type[3]++;
                                pc -= imm/4;
                            }
                        }  else {
                            pc = 1;
                        }
                    } else if (funct3 == 0) {  // beq
                       //printf("beq\n");
                        if(get_register(rs1) == get_register(rs2)){
                           printf("beq: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[0]++;
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x1){  // bne
                        if(get_register(rs1) != get_register(rs2)){
                           printf("bne: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[1]++;
                            //printf("pc:%d\n",pc);
                            pc += imm/4;
                            //printf("after_pc:%d\n",pc);
                        }
                    } else if(funct3 == 0x4){  // blt
                        if(get_register(rs1) < get_register(rs2)){
                           printf("blt: x%d, x%d, %d\n", rs1, rs2, imm);
                           counter.b_type[2]++;
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x5){  // bge
                        printf("bge");
                        printf("rs1: %d, rs2: %d\n",get_register(rs1),get_register(rs2));
                        counter.b_type[3]++;
                        if(get_register(rs1) >= get_register(rs2)){
                           printf("bge: x%d, x%d, %d\n", rs1, rs2, imm);
                            pc += imm/4;
                        }
                    }
                    //printf("pc: 361: %d\n",pc);
                    if(pc == 0){
                        pc = 1;
                    }
                    pc_operand.pc = pc;
                }
                return pc_operand;

            case 0x5: //lui
                {
                    printf("u_type\n");
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t bit31_12 = (instruction >> 12) & 0xFFFFF;
                    uint32_t value = bit31_12 << 12 / 4; //これ4で割らないといけないかもしれない。4で割るなら行数と一致する
                    set_register(rd,value);
                    counter.u_type[0]++;
                }
                return pc_operand;

            case 0x6: //auipc {upimm, 12'b0} + pc
                {
                    printf("auipc\n");
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t bit31_12 = (instruction >> 12) & 0xFFFFF;
                    printf("%x\n",bit31_12);
                    uint32_t value = (bit31_12 << 12); //これも4で割るべきかも？ それか下のcurrent_lineを4倍する
                    if(bit31_12 & 0x80000){
                        //負の値
                        uint32_t mask = (1 << 32) - 1;
                        value = ~value & mask;
                        value = value + 1;
                    }
                    printf("value:%d\n",value);
                    value = value / 4;
                    value = value + current_line; 
                    set_register(rd,value);
                    counter.aui_type[0]++;
                    printf("auipc x%d ,%d\n",rd,bit31_12);
                }
                return pc_operand;

            case 0x7:  // J形式命令 (例: "jal")
                {
                    printf("jal\n");
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
                    printf("Executed jal: x%d = PC + 1, PC += %d\n", rd, imm);
                    // 符号拡張
                    if (bit20) {
                        imm |= 0xFFE00000;  // 負の値の場合、上位ビットを1で埋める
                    }
                    int next_line = current_line + imm/4;
                    printf("imm: %d\n",imm);
                    //printf("jal: x%d, %d\n", rd, imm);
                    printf("jal x%d, %d (PC: %d -> %d)\n", rd, imm, current_line +1, next_line +1);
                    counter.j_type[0]++;
                    // 戻りアドレスを保存
                    printf("戻りアドレスrd: %d\n",current_line+2);
                    set_register(rd, current_line + 2);
                    printf("戻りアドレスrd: %d\n",get_register(rd));
                    
                    // PCの更新後に即座にreturn
                    pc = imm/4;
                    pc_operand.pc = pc;
                }
                return pc_operand;
                
            case 0x8:  // I形式命令 ("jalr")
                {   
                   printf("i_type (jalr)\n");
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
                        printf("immが正");
                        printf("jalr: x%d, x%d, %d\n", rd, rs1, imm);
                        pc = get_register(rs1) + imm/4 - current_line - 1;
                        set_register(rd, get_register(rs1) + imm/4);
                        printf("rs1:%d\n",get_register(rs1));
                        printf("pc:%d\n",pc);
                        printf("rd:x%d = %d\n",rd,get_register(rd));
                    }else if(minus == 1){
                        printf("jalr: x%d, x%d, -%d\n", rd, rs1, imm);
                        pc = get_register(rs1) - imm/4 - current_line - 1;
                        set_register(rd, get_register(rs1) - imm/4);
                        printf("rs1:%d\n",get_register(rs1));
                        printf("pc:%d\n",pc);
                    }
                    printf("x1 = %d\n",get_register(1));
                }
               printf("pc:%d\n",pc);
                if(pc == 0){
                    pc = 1;
                }
                pc_operand.pc = pc;
                return pc_operand;

            case 0x9:  // lw  x[rd] = mem[x[r1] + offset]
                {
                   printf("lw\n");
                   counter.lw_type[0]++;
                    uint32_t rs1 = (instruction >> 13) & 0x3F;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t lw_offset = (instruction >> 20) & 0xFFF;
                    printf("rs1:x%d,rd;x%d,lw_offset:%d\n",rs1,rd,lw_offset);
                    int lw = 0; //setする値
                    pc_operand.opcode = 1;
                    pc_operand.operand1 = rd;
                    pc_operand.operand2 = rs1;
                    if(lw_offset && 0x800 == 1){//負の値
                        uint32_t mask = (1<<12) - 1;
                        lw_offset = ~lw_offset & mask;
                        lw_offset = lw_offset + 1;
                        lw = memory[get_register(rs1) - lw_offset];
                       printf("lw: x%d, -%d(x%d)\n", rd, lw_offset, rs1);
                    }else{
                        printf("正\n");
                        printf("address:%d + %d\n",get_register(rs1),lw_offset);
                        lw = memory[get_register(rs1) + lw_offset];
                       printf("lw: x%d, %d(x%d)\n", rd, lw_offset, rs1);
                    }
                    printf("memory%dの中に格納されている値:%d\n",get_register(rs1) + lw_offset,lw);
                    fprintf(memory_file,"memory%dの中に格納されている値:%d\n",get_register(rs1) + lw_offset,lw);
                    set_register(rd,lw);
                }   
                return pc_operand;
            
            case 0xa:   //fadd,fsub,fmul,fdiv
                {   
                    printf("f_type\n");
                    pc_operand.pc = 1;
                    //func: 0:fadd, 1:fsub, 2:fmul, 3:fdiv, 10:finv, 11: fsqrt
                   //printf("f形式");
                    uint32_t func = (instruction >> 27) & 0x1F;
                    uint32_t rd = (instruction >> 4) & 0x3F;
                    uint32_t r1 = (instruction >> 13) & 0x3F;
                    uint32_t r2 = (instruction >> 19) & 0x3F;
                    float a1 = get_float_register(r1);
                    float a2 = get_float_register(r2);
                    float result;
                    if(func == 0){
                        result = fadd(a1,a2);
                        printf("result:%f\n",result);
                        printf("fadd x%d x%d\n",r1,r2);
                        printf("result %f + %f = %f\n",a1,a2,result);
                        set_register(rd, result);
                        counter.f_type[0]++;
                    }
                    if(func == 1){
                        result = fsub(a1,a2);
                        printf("fsub x%d = x%d - x%d\n",rd, r1,r2);
                        printf("result %f - %f = %f\n",a1,a2,result);
                        set_register(rd, result);
                        counter.f_type[1]++;
                    }
                    if(func == 2){
                        result = fmul(a1,a2);
                        set_register(rd, result);
                        counter.f_type[2]++;
                    }
                    if(func == 3){
                        result = fdiv(a1,a2);
                        set_register(rd, result);
                        counter.f_type[3]++;
                    }
                    if(func == 10){
                        result = finv(a1,a2);
                        set_register(rd, result);
                        counter.f_type[4]++;
                    }
                    if(func == 11){
                        result = fsqrt(a1);
                        set_register(rd, result);
                        counter.f_type[5]++;
                    }

                    // rdにresultを格納
                    set_register(rd, result);
                }
                return pc_operand;
                
            case 0xb:  // csrr, csrw
                {
                     printf("csrr/csrw\n");
                    uint32_t func = (instruction >> 10) & 0x7;
                    //csrw
                    if(func == 1){ // x10の値をファイルに書きこむ
                        double value = get_register(10);
                        printf("%fの値をファイルに書き込む",value);
                        fprintf(sld_result_file, "%f\n", value);
                        counter.c_type[1]++;
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

void print_register_transition(FILE *transition_file, FILE *float_transition_file, int pc){
    fprintf(transition_file, "| ");
    fprintf(transition_file, "%2d行|",pc);    
    fprintf(float_transition_file, "| ");
    fprintf(float_transition_file, "%2d行|",pc);
    for(int i = 0; i< 32; i++) {
        fprintf(transition_file, "%3d | ", get_register(i));
    }    
    for(int i = 32; i< NUM_REGISTERS; i++) {
        fprintf(float_transition_file, "%3f | ", get_float_register(i));
    }
    fprintf(transition_file, "\n");
    fprintf(float_transition_file, "\n");
}

void for_markdown(FILE *transition_file){
    // Markdownの表ヘッダーを出力
    fprintf(transition_file, "| ");
    fprintf(transition_file, "実行命令|");
    for (int i = 0; i < 32; i++) {
        fprintf(transition_file, "x%-2d | ", i);
    }
    fprintf(transition_file, "\n|");

    // 区切り線を出力
    for (int i = 0; i < NUM_REGISTERS+1; i++) {
        fprintf(transition_file, "---:|");
    }
    fprintf(transition_file, "\n");
    fflush(transition_file);
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

    for_markdown(transition_file);

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

