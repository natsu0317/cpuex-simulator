// binary から 実行結果を出力
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 32
#define MAX_INSTRUCTIONS 100 //読み取る最大行数
#define INSTRUCTION_LENGTH 33 //32bit + 終端文字

// registerのsimulation
int registers[NUM_REGISTERS] = {0};  // 32個のレジスタを初期化

// レジスタの値を設定する
void set_register(int reg_num, int value) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < NUM_REGISTERS) {
        registers[reg_num] = value;
    }
}

// レジスタの値を取得する
int get_register(int reg_num) {
    registers[0] = 0; //x0は常に0
    if (reg_num >= 1 && reg_num < NUM_REGISTERS) {
        return registers[reg_num];
    }
    printf("reg[%d] = %d\n",reg_num,registers[reg_num]);
    return 0;
}

// バイナリ命令をデコードして処理
void execute_binary_instruction(const char binary_instruction[][33], int num_instructions) {
    for(int pc=0; pc<num_instructions; pc++){
        printf("pc:%d\n",pc);
        printf("x1:%d\n",get_register(1));
        uint32_t instruction = strtol(binary_instruction[pc], NULL, 2); //2進数文字列を数値に変換

        // オペコードを取得
        //下7桁(F=15)
        uint32_t opcode = instruction & 0x7F;

        switch (opcode) {
            case 0x0:   //label部分
                {

                }
                break;
            case 0x33:  // R形式命令 (例:"add", "sub", "and", "or", "xor",)
                {
                    uint32_t funct3 = (instruction >> 12) & 0x7;
                    uint32_t funct7 = (instruction >> 25) & 0x7F;
                    uint32_t rd = (instruction >> 7) & 0x1F;
                    uint32_t rs1 = (instruction >> 15) & 0x1F;
                    uint32_t rs2 = (instruction >> 20) & 0x1F;

                    if (funct3 == 0 && funct7 == 0) {  // add命令
                        set_register(rd, get_register(rs1) + get_register(rs2));
                        printf("Executed add: x%d = x%d + x%d\n", rd, rs1, rs2);
                        printf("result:%d\n",get_register(rd));
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

            case 0x13:  // I形式命令 (例: "addi", "andi", "ori", "xori", "jalr")
                {
                    uint32_t funct3 = (instruction >> 12) & 0x7;
                    uint32_t rd = (instruction >> 7) & 0x1F;
                    uint32_t rs1 = (instruction >> 15) & 0x1F;
                    int32_t imm = (int32_t)(instruction >> 20) & 0xFF;
                    int32_t opcode = (instruction >> 0) & 0x7F;
                    printf("imm:%x\n",imm);
                    if(imm & 0x800){
                        //即値が負の値
                        uint32_t mask = (1 << 12) - 1;
                        imm = ~imm & mask;//反転
                        imm = imm+1;
                    }
                    printf("after_imm:%x\n",imm);
                    printf("funct3:%x\n,opcode:%x\n",funct3,opcode);

                    if (funct3 == 0 && opcode == 0x13) {  // addi命令
                        set_register(rd, get_register(rs1) + imm);
                        printf("Executed addi: x%d = x%d + %d\n", rd, rs1, imm);
                        printf("result:%d\n",get_register(rd));
                    } else if (funct3 == 0x7 && opcode == 0x13){  // andi
                        set_register(rd, get_register(rs1) & imm);
                        printf("Executed and: x%d = x%d & x%d\n", rd, rs1, imm);
                    } else if (funct3 == 0x6 && opcode == 0x13){  // ori
                        set_register(rd, get_register(rs1) | imm);
                        printf("Executed ori: x%d = x%d | x%d\n", rd, rs1, imm);
                    } else if (funct3 == 0x4 && opcode == 0x13){  // xori
                        set_register(rd, get_register(rs1) ^ imm);
                        printf("Executed xori: x%d = x%d ^ x%d\n", rd, rs1, imm);
                    } else if (funct3 == 0x0 && opcode == 0x67){  //jalr
                        set_register(rd, pc+1);
                        pc = get_register(rs1) + imm/4;
                        pc -= 1;
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
                    printf("imm:%x\n",imm);
                    if(bit12 == 1){
                        //immは負の値
                        //2の補数
                        uint32_t mask = (1 << 12) -1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                        printf("imm:%x\n",imm);
                        if (funct3 == 0) {  // beq
                            if(get_register(rs1) == get_register(rs2)){
                                printf("beq\n");
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x1){  // bne
                            if(get_register(rs1) != get_register(rs2)){
                                printf("bne\n");
                                printf("pc:%d\n",pc);
                                pc -= imm/4;
                                printf("after_pc:%d\n",pc);
                            }
                        } else if(funct3 == 0x4){  // blt
                            if(get_register(rs1) < get_register(rs2)){
                                printf("blt\n");
                                pc -= imm/4;
                            }
                        } else if(funct3 == 0x5){  // bge
                            if(get_register(rs1) >= get_register(rs2)){
                                printf("bge\n");
                                pc -= imm/4;
                            }
                        }  
                    }

                    if (funct3 == 0) {  // beq
                        if(get_register(rs1) == get_register(rs2)){
                            printf("beq\n");
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x1){  // bne
                        if(get_register(rs1) != get_register(rs2)){
                            printf("bne\n");
                            printf("pc:%d\n",pc);
                            pc += imm/4;
                            printf("after_pc:%d\n",pc);
                        }
                    } else if(funct3 == 0x4){  // blt
                        if(get_register(rs1) < get_register(rs2)){
                            printf("blt\n");
                            pc += imm/4;
                        }
                    } else if(funct3 == 0x5){  // bge
                        if(get_register(rs1) >= get_register(rs2)){
                            printf("bge\n");
                            pc += imm/4;
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
                    imm |= (bit10_1 << 1);
                    printf("jal\n");
                    if(bit20 == 1){
                        //immが負の値
                        uint32_t mask = (1 << 20) - 1;
                        imm = ~imm & mask;
                        imm = imm + 1;
                        set_register(rd,pc+1);
                        pc -= imm/4;
                    } else {
                        set_register(rd, pc+1);
                        printf("imm:%d\n",imm);
                        printf("before_pc:%d\n",pc);
                        pc += imm/4;
                        printf("after_pc:%d\n",pc);
                    }
                }
                break;

        }
    }

}

void print_register(FILE* output_file){
    for(int i=0;i<32;i++){
        fprintf(output_file, "x%d = %d\n", i, get_register(i));
    }
}


int main() {
    char binary_instructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH];  // 読み取った命令を格納する配列
    FILE *file = fopen("instruction.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file\n");
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

    fclose(file);
    printf("%d\n",index);
    execute_binary_instruction(binary_instructions,index);

    // レジスタの状態を表示
    FILE *output_file = fopen("result.txt","w");
    if(output_file == NULL){
        perror("Error opening file");
        return 1;
    }
    print_register(output_file);
    fclose(output_file);

    return 0;
}

