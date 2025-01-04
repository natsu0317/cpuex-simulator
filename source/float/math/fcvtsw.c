#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// 32ビット整数を単精度浮動小数点数に変換
float fcvtsw(int32_t x) {
    if (x == 0) {
        return 0.0f;
    }

    uint32_t abs_x = (x < 0) ? -x : x; // 符号を無視して絶対値を取得
    uint32_t sign = (x < 0) ? 0x80000000 : 0; // 符号ビット

    // 最上位ビットの位置を探す
    int shift = 0;
    while ((abs_x & (1 << 31)) == 0) {
        abs_x <<= 1;
        shift++;
    }

    // 仮数部の計算（23ビット）
    uint32_t mantissa = (abs_x & 0x7FFFFFFF) >> 8;

    // 丸め処理（24ビット目に基づく）
    if (abs_x & (1 << 7)) {
        mantissa++;
        if (mantissa & 0x800000) { // 桁あふれ
            mantissa = 0;
            shift--;
        }
    }

    // 指数部の計算
    uint32_t exponent = 127 + 31 - shift;

    // 浮動小数点数のビット表現を組み立てる
    uint32_t float_bits = sign | (exponent << 23) | (mantissa & 0x7FFFFF);

    float result;
    memcpy(&result, &float_bits, sizeof(result));

    return result;
}

// void test_fcvtsw() {
//     // テストする整数値
//     int32_t test_values[] = {
//         0, 1, -1, 123, -456, INT32_MAX, INT32_MIN, 
//         999999, -999999, 1000000000, -1000000000
//     };
//     int num_tests = sizeof(test_values) / sizeof(test_values[0]);

//     // 各テスト値に対して結果を確認
//     for (int i = 0; i < num_tests; i++) {
//         int32_t x = test_values[i];
//         float result = fcvtsw(x);

//         // 結果を表示
//         printf("fcvtsw(%d) = %.7g\n", x, result);

//         // 精度をチェック
//         float diff = fabs(result - (float)x);
//         if (diff > FLT_EPSILON) {
//             printf("Test failed for x = %d: |%.7g - %.7g| = %.7g\n",
//                    x, result, (float)x, diff);
//         }
//     }
// }

// // メイン関数
// int main() {
//     printf("Testing fcvtsw:\n");
//     test_fcvtsw();
//     return 0;
// }