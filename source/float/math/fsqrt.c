#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// グローバル変数として定義
float a_table_sqrt[1024]; // 傾き
float b_table_sqrt[1024]; // y切片

void init_ab_sqrt() {
    float e = 3.0f / 1024.0f;
    for (int i = 0; i < 1024; i++) {
        float x1 = 1.0f + i * e;
        float x2 = 1.0f + (i + 1) * e;
        float x3 = 1.0f + (i + 0.5f) * e;
        a_table_sqrt[i] = (sqrt(x2) - sqrt(x1)) / e; // 傾き
        float x4 = (sqrt(x1) + sqrt(x2)) / 2.0f; // 平均値
        b_table_sqrt[i] = x4 - a_table_sqrt[i] * x3; // y切片
    }
}

float fsqrt(float x) {
    if (x < 0.0f) {
        return NAN; // 負の数は平方根が定義されない
    }
    if (x == 0.0f) {
        return 0.0f; // 0 の平方根は 0
    }

    // 初期化
    static int initialized = 0;
    if (!initialized) {
        init_ab_sqrt();
        initialized = 1;
    }

    uint32_t x_bits;
    memcpy(&x_bits, &x, sizeof(x_bits));

    uint32_t s = (x_bits >> 31) & 0x1; // 符号ビット
    uint32_t e = (x_bits >> 23) & 0xFF; // 指数部
    uint32_t m = x_bits & 0x7FFFFF; // 仮数部

    // 正規化
    float x_prime = 1.0f + (m / (float)(1 << 23)); // [1.0, 2.0) に変換
    uint32_t index = (uint32_t)((x_prime - 1.0f) * 1024.0f); // テーブルのインデックス計算

    float a_value = a_table_sqrt[index];
    float b_value = b_table_sqrt[index];

    // 近似値
    float sqrt_x_prime = a_value * x_prime + b_value;

    // 指数の調整
    int exp_adjust = (e - 127) / 2 + 127;
    if ((e - 127) % 2 != 0) { // 奇数の指数の場合
        sqrt_x_prime *= sqrt(2.0f);
    }

    // 結果を再構築
    uint32_t result_bits;
    memcpy(&result_bits, &sqrt_x_prime, sizeof(sqrt_x_prime));
    result_bits = (s << 31) | (exp_adjust << 23) | (result_bits & 0x7FFFFF);

    float result;
    memcpy(&result, &result_bits, sizeof(result));

    return result;
}

// int main() {
//     float a = 1.23f;
//     float b = 2.34f;
//     float result = fsqrt(a);
//     printf("Result of fsqrt(%f) = %f\n", a, result);
//     result = fsqrt(b);
//     printf("Result of fsqrt(%f) = %f\n", b, result);
//     return 0;
// }
