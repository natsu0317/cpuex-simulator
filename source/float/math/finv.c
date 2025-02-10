#include "math_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

float fmul(float a, float b);
float fsub(float a, float b);

// グローバル変数
float a_table[1024]; // 傾き
float b_table[1024]; // y切片
bool is_initialized = false; // 初期化フラグ

void init_ab() {
    if (is_initialized) return; // 既に初期化されている場合は何もしない

    float e = 1.0f / 1024.0f;
    for (int i = 0; i < 1024; i++) {
        float x1 = 1.0f + i * e;
        float x2 = 1.0f + (i + 1) * e;
        float x3 = 1.0f + (i + 0.5f) * e;
        a_table[i] = fabs((1.0f / x2 - 1.0f / x1) / e); // 傾きの絶対値
        float x4 = ((1.0f / x1 + 1.0f / x2) / 2.0f + 1.0f / x3) / 2.0f; // 平均値
        b_table[i] = x4 - a_table[i] * x3; // y切片
    }

    is_initialized = true; // 初期化完了
}

// プログラム開始時に呼び出す関数
void initialize_finv_table() {
    init_ab();
}

float finv(float a) {
    // init_ab() の呼び出しを削除

    uint32_t a_bits;
    memcpy(&a_bits, &a, sizeof(a_bits));

    uint32_t s = (a_bits >> 31) & 0x1;
    uint32_t e = (a_bits >> 23) & 0xFF;
    uint32_t m = a_bits & 0x7FFFFF;

    if (e == 0xFF) {
        return NAN; // 無限大またはNaNの場合はエラー
    }

    uint32_t einv = 253 - e; // 逆数の指数計算
    uint32_t idx = m >> 13;  // 上位10ビットをインデックスに使用

    float a_value = a_table[idx];
    float b_value = b_table[idx];

    // 浮動小数点値を計算
    float x = 1.0f + (m / (float)(1 << 23)); // 仮数を正規化
    float minv = fsub(b_value, fmul(a_value, x)); // 逆数の仮数を計算

    // 結果を再構築
    uint32_t minv_bits;
    memcpy(&minv_bits, &minv, sizeof(minv_bits));
    uint32_t result_bits = (s << 31) | (einv << 23) | (minv_bits & 0x7FFFFF);

    float result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;
}

// int main() {
//     initialize_program(); // プログラム開始時に一度だけ初期化

//     float a = 1.23f;
//     float b = 2.34f;
//     float result = finv(a);
//     printf("Result of finv(%f) = %f\n", a, result);
//     result = finv(b);
//     printf("Result of finv(%f) = %f\n", b, result);
//     return 0;
// }