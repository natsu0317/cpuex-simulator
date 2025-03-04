// gcc -o compare compare.c
// ./compare ./document/float_result.txt ./document/memory_transition.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 32ビット文字列を浮動小数点数に変換する関数
float binary32_to_float(const char *binary) {
    uint32_t bits = 0;
    for (int i = 0; i < 32; i++) {
        if (binary[i] == '1') {
            bits |= (1 << (31 - i));
        }
    }
    
    float result;
    memcpy(&result, &bits, sizeof(result));
    return result;
}

// ファイルの比較関数
void compare_binary_float_files(const char *file1_path, const char *file2_path, FILE* diff) {
    FILE *file1 = fopen(file1_path, "r");
    if (!file1) {
        fprintf(stderr, "Error opening file: %s\n", file1_path);
        return;
    }
    
    FILE *file2 = fopen(file2_path, "r");
    if (!file2) {
        fprintf(stderr, "Error opening file: %s\n", file2_path);
        fclose(file1);
        return;
    }
    
    char line1[100], line2[100];
    int line_number = 0;
    int mismatch_count = 0;
    
    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        line_number++;
        
        // 改行文字を削除
        line1[strcspn(line1, "\r\n")] = 0;
        line2[strcspn(line2, "\r\n")] = 0;
        
        // 行の長さが32ビットでない場合はスキップ
        if (strlen(line1) != 32 || strlen(line2) != 32) {
            fprintf(stderr, "Line %d: Invalid format (not 32 bits)\n", line_number);
            continue;
        }
        
        // 32ビット表現を浮動小数点数に変換
        float value1 = binary32_to_float(line1);
        float value2 = binary32_to_float(line2);
        
        // ビット表現が異なる場合に出力
        if (strcmp(line1, line2) != 0) {
            mismatch_count++;
            fprintf(diff,"Mismatch at line %d:\n", line_number);
            fprintf(diff,"  File1: %s (%.9f)\n", line1, value1);
            fprintf(diff,"  File2: %s (%.9f)\n", line2, value2);
            fprintf(diff,"  Difference: %.9f\n\n", value1 - value2);
        }
    }
    
    // ファイルの長さが異なる場合
    if (fgets(line1, sizeof(line1), file1)) {
        printf("File1 has more lines than File2\n");
    }
    
    if (fgets(line2, sizeof(line2), file2)) {
        printf("File2 has more lines than File1\n");
    }
    
    printf("Total mismatches: %d\n", mismatch_count);
    
    fclose(file1);
    fclose(file2);
}

int main(int argc, char *argv[]) {
    FILE *diff = fopen("./diff.txt","w");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }
    
    compare_binary_float_files(argv[1], argv[2], diff);
    return 0;
}