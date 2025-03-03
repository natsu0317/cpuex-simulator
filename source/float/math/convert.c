// fpuで書かれたfinv_table,fsqrt_tableのa,bを16進数に変換し結合


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// 2進数文字列をuint32_tに変換する関数
uint32_t binary_to_uint32(const char *binary) {
    uint32_t result = 0;
    while (*binary) {
        result = (result << 1) | (*binary - '0');
        binary++;
    }
    return result;
}

int main() {
    FILE *input_file = fopen("fsqrt_table.txt", "r");
    if (!input_file) {
        perror("ファイルを開けませんでした");
        return 1;
    }

    FILE *output_file = fopen("fsqrt_table_hex.c", "w");
    if (!output_file) {
        perror("出力ファイルを作成できませんでした");
        fclose(input_file);
        return 1;
    }

    // ヘッダーを書き込む
    fprintf(output_file, "const uint64_t inv_table[] = {\n");

    char line1[40];  // 32ビット + 余裕
    char line2[40];
    int pair_count = 0;

    // 2行ずつ読み込む
    while (fgets(line1, sizeof(line1), input_file)) {
        // 改行文字を削除
        line1[strcspn(line1, "\r\n")] = 0;
        
        // 空行をスキップ
        if (strlen(line1) == 0) continue;
        
        // 2行目を読み込む
        if (!fgets(line2, sizeof(line2), input_file)) {
            fprintf(stderr, "警告: 奇数行のファイルです。最後の行をスキップします。\n");
            break;
        }
        line2[strcspn(line2, "\r\n")] = 0;
        
        // 空行をスキップ
        if (strlen(line2) == 0) continue;

        // 2進数を32ビット整数に変換
        uint32_t a = binary_to_uint32(line1);
        uint32_t b = binary_to_uint32(line2);
        
        // 64ビットの値を作成
        uint64_t combined = ((uint64_t)a << 32) | b;
        
        // 16進数で出力
        fprintf(output_file, "    0x%016llx,", (unsigned long long)combined);
        
        // コメントとして元の2進数を追加（オプション）
        // fprintf(output_file, " // %s, %s", line1, line2);
        fprintf(output_file, "\n");
        
        pair_count++;
    }

    fprintf(output_file, "};\n\n");
    fprintf(output_file, "// Total pairs: %d\n", pair_count);

    fclose(input_file);
    fclose(output_file);
    
    printf("変換が完了しました。%d 組のデータを変換しました。\n", pair_count);
    
    return 0;
}