// binary から 実行結果を出力
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_REGISTERS 32

// registerのsimulation
int registers[NUM_REGISTERS] = {0};  // 32個のレジスタを初期化

// レジスタの値を設定する
void set_register(int reg_num, int value) {
    if (reg_num >= 0 && reg_num < NUM_REGISTERS) {
        registers[reg_num] = value;
    }
}

// レジスタの値を取得する
int get_register(int reg_num) {
    if (reg_num >= 0 && reg_num < NUM_REGISTERS) {
        return registers[reg_num];
    }
    return 0;
}

// バイナリ命令をデコードして処理
void execute_binary_instruction(const char* binary_instruction[], int num_instructions) {
    for(int pc=0; pc<num_instructions; pc++){
        uint32_t instruction = strtol(binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換

        // オペコードを取得
        //下7桁(F=15)
        uint32_t opcode = instruction & 0x7F;

        switch (opcode) {
            case 0x33:  // R形式命令 (例:"add", "sub", "and", "or", "xor")
                {
                    uint32_t funct3 = (instruction >> 12) & 0x7;
                    uint32_t funct7 = (instruction >> 25) & 0x7F;
                    uint32_t rd = (instruction >> 7) & 0x1F;
                    uint32_t rs1 = (instruction >> 15) & 0x1F;
                    uint32_t rs2 = (instruction >> 20) & 0x1F;

                    if (funct3 == 0 && funct7 == 0) {  // add命令
                        set_register(rd, get_register(rs1) + get_register(rs2));
                        printf("Executed add: x%d = x%d + x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0 && funct7 == 0x20) {  // sub命令
                        set_register(rd, get_register(rs1) - get_register(rs2));
                        printf("Executed sub: x%d = x%d - x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x7 && funct7 == 0){  // and
                        set_register(rd, get_register(rs1) & get_register(rs2));
                        printf("Executed and: x%d = x%d & x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x6 && funct7 == 0){  // or
                        set_register(rd, get_register(rs1) | get_register(rs2));
                        printf("Executed or: x%d = x%d | x%d\n", rd, rs1, rs2);
                    } else if (funct3 == 0x4 && funct7 == 0){  // xor
                        set_register(rd, get_register(rs1) ^ get_register(rs2));
                        printf("Executed xor: x%d = x%d ^ x%d\n", rd, rs1, rs2);
                    }
                }
                break;

            case 0x13:  // I形式命令 (例: "addi", "andi", "ori", "xori")
                {
                    printf("instruction:%x\n",instruction);
                    uint32_t funct3 = (instruction >> 12) & 0x7;
                    uint32_t rd = (instruction >> 7) & 0x1F;
                    uint32_t rs1 = (instruction >> 15) & 0x1F;
                    int32_t imm = (int32_t)(instruction >> 20);
                    if(imm & 0x800){
                        //即値が負の値
                        uint32_t mask = (1 << 12) - 1;
                        imm = ~imm & mask;//反転
                        imm = imm+1;
                    }

                    if (funct3 == 0) {  // addi命令
                        set_register(rd, get_register(rs1) + imm);
                        printf("Executed addi: x%d = x%d + %d\n", rd, rs1, imm);
                    } else if (funct3 == 0x7){  // andi
                        set_register(rd, get_register(rs1) & imm);
                        printf("Executed and: x%d = x%d & x%d\n", rd, rs1, imm);
                    } else if (funct3 == 0x6){  // ori
                        set_register(rd, get_register(rs1) | imm);
                        printf("Executed ori: x%d = x%d | x%d\n", rd, rs1, imm);
                    } else if (funct3 == 0x4){  // xori
                        set_register(rd, get_register(rs1) ^ imm);
                        printf("Executed xori: x%d = x%d ^ x%d\n", rd, rs1, imm);
                    }
                }
                break;

            case 0x63:  // B形式命令 (例: "beq", "bne", "blt", "bge")
                {
                    uint32_t funct3 = (instruction >> 12) & 0x7;
                    uint32_t rs1 = (instruction >> 15) & 0x1F;
                    uint32_t rs2 = (instruction >> 20) & 0x1F;
                    uint32_t bit12 = (instruction >> 31) & 0x1;
                    uint32_t bit10_5 = (instruction >> 25) & 0x3F;
                    uint32_t bit4_1 = (instruction >> 8) & 0xF;
                    uint32_t bit11 = (instruction >> 7) & 0x1;
                    uint32_t imm = 0;
                    imm |= (bit12 << 12);
                    imm |= (bit11 << 11);
                    imm |= (bit10_5 << 5);
                    imm |= (bit4_1 << 1);
                    printf("%x:imm\n",imm);
                    if(bit12 == 1){
                        //immは負の値
                        //2の補数
                        uint32_t mask = (1 << 12) -1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                        printf("imm:%x\n",imm);
                        if (funct3 == 0) {  // beq
                            if(get_register(rs1) == get_register(rs2)){
                                pc -= imm;
                            }
                        } else if(funct3 == 0x1){  // bne
                            if(get_register(rs1) != get_register(rs2)){
                                pc -= imm;
                            }
                        } else if(funct3 == 0x4){  // blt
                            if(get_register(rs1) < get_register(rs2)){
                                pc -= imm;
                            }
                        } else if(funct3 == 0x5){  // bge
                            if(get_register(rs1) >= get_register(rs2)){
                                pc -= imm;
                            }
                        }  
                    }

                    if (funct3 == 0) {  // beq
                        if(get_register(rs1) == get_register(rs2)){
                            pc += imm;
                        }
                    } else if(funct3 == 0x1){  // bne
                        if(get_register(rs1) != get_register(rs2)){
                            pc += imm;
                        }
                    } else if(funct3 == 0x4){  // blt
                        if(get_register(rs1) < get_register(rs2)){
                            pc += imm;
                        }
                    } else if(funct3 == 0x5){  // bge
                        if(get_register(rs1) >= get_register(rs2)){
                            pc += imm;
                        }
                    }  
                }
                break;
                  

            case 0x6F:  // J形式命令 (例: "jal")
                {
                    uint32_t bit20 = (instruction >> 31) & 0x1;
                    uint32_t bit10_1 = (instruction >> 21) & 0x3FF;
                    uint32_t bit11 = (instruction >> 20) & 0x1;
                    uint32_t bit19_12 = (instruction >> 12) & 0xFF;
                    uint32_t rd = (instruction >> 7) & 0x1F;
                    uint32_t imm = 0;
                    imm |= (bit20 << 20);
                    imm |= (bit19_12 << 12);
                    imm |= (bit11 << 11);
                    imm |= (bit10_1 << 10);
                    if(bit20 == 1){
                        //immが負の値
                        uint32_t mask = (1 << 20) - 1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                        set_register(rd,pc+4);
                        pc -= imm;
                    } else {
                        set_register(rd, pc+4);
                        pc += imm;
                    }
                }
                break;

        }
    }

}


int main() {
    const char* binary_instructions[] = {
        "00000000001100010000000010110011",
        "11111001110000010000000010010011",
        "11111110001000001000110011100011",//jal x1, -100
        // 他のバイナリ命令
    };

    int num_instructions = sizeof(binary_instructions) / sizeof(binary_instructions[0]);

    execute_binary_instruction(binary_instructions,num_instructions);

    // レジスタの状態を表示
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("x%d = %d\n", i, get_register(i));
    }

    return 0;
}

