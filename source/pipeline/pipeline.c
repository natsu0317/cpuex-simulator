#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../asm_to_binary/asm_to_binary.h"

#define MAX_ASSEMBLY_SIZE 1024 //アセンブリコードの最大サイズ

//計算機構成論week4
//fetch : 命令を取ってくる
//decode : レジスタファイルを読み出して命令をデコード
//register fetch :
//execute : 実行
//memory access :
//write back :

//IF -> ID -> EX -> MEM -> WB

int main(){
    //ファイルからassembly取得
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
    printf("assmbly_code:%s\n",assembly_code);
    //assmblyをbinaryに変換
    remove_comments(assembly_code);

    parse_assembly(assembly_code);

    //binary codeはbinary.txtにoutput
    FILE *output_file = fopen("binary.txt","w");    
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }
    print_binary_instructions(output_file);
    fclose(output_file);
    return 0;
}