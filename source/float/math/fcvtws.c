#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// 浮動小数点数を 32 ビット整数に変換する関数
int32_t fcvtws(float x) {
    // 特殊ケース処理（0.0 の場合）
    if (x == 0.0f) {
        return 0;
    }

    // 浮動小数点数をビット列に変換
    uint32_t x_bits;
    memcpy(&x_bits, &x, sizeof(x_bits));

    // 符号（S）、指数部（E）、仮数部（M）の抽出
    uint32_t sign = (x_bits >> 31) & 0x1;         // 符号ビット
    int32_t exponent = ((x_bits >> 23) & 0xFF) - 127; // 指数部 - バイアス
    uint32_t mantissa = x_bits & 0x7FFFFF;       // 仮数部

    // 仮数部の MSB に「隠れた1」を補う（ケチビット）
    uint32_t extended_mantissa = (1 << 23) | mantissa;

    // 指数部の値に基づいて仮数をシフト
    if (exponent >= 31) {
        // 範囲外（オーバーフロー）
        return sign ? INT32_MIN : INT32_MAX;
    } else if (exponent < 0) {
        // 範囲外（切り捨て）
        return 0;
    }

    // ビットシフトで整数部分を取得
    uint32_t integer_part;
    if (exponent >= 23) {
        integer_part = extended_mantissa << (exponent - 23);
    } else {
        uint32_t fractional_part = extended_mantissa & ((1 << (23 - exponent)) - 1);
        integer_part = extended_mantissa >> (23 - exponent);

        // 0捨1入による丸め
        if (fractional_part >= (1U << (22 - exponent))) {
            integer_part += 1;
        }
    }

    // 符号ビットの処理（負数の場合は 2 の補数に変換）
    int32_t result = sign ? -(int32_t)integer_part : (int32_t)integer_part;

    return result;
}

// int main() {
//     float test_values[] = {0.0f, 1.5f, -2.5f, 123456.78f, -98765.43f, 1.999f, -0.999f};
//     int num_tests = sizeof(test_values) / sizeof(test_values[0]);

//     for (int i = 0; i < num_tests; i++) {
//         float x = test_values[i];
//         int32_t result = fcvtws(x);
//         printf("fcvtws(%f) = %d\n", x, result);
//     }

//     return 0;
// }
