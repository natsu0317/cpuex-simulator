#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// 仮のブロックRAMからaとbを取得する関数
void get_ab_from_ram(uint32_t index, float* a, float* b) {
    // ここでインデックスに基づいて適切なaとbを設定
    // 例:
    *a = 1.0f; // 仮の値
    *b = 0.0f; // 仮の値
}

float fsqrt(float x){
    uint32_t x_bits;
    memcpy(&x_bits, &x, sizeof(x_bits));

    uint32_t s = (x_bits >> 31) & 0x1;
    uint32_t e = (x_bits >> 23) & 0xFF;
    uint32_t m = x_bits & 0x7FFFFF;

    // 正規化して[1.0, 4.0)の範囲にする
    if (e % 2 == 1) {
        // 奇数指数の場合
        e -= 1;
        m = (m >> 1) | (1 << 22);
    }

    // インデックス計算
    uint32_t index = ((e & 0x1) << 9) | (m >> 14);

    // ブロックRAMからaとbを取得
    float a, b;
    get_ab_from_ram(index, &a, &b);

    // 近似解の計算
    float x_prime = (1.0f + (float)m / (1 << 23));
    float sqrt_x_prime = fadd(b, fmul(a, x_prime));

    // 最終結果の計算
    e = (e >> 1) + 63; // 2で割った後にバイアスを調整
    uint32_t result_bits = (s << 31) | (e << 23) | ((uint32_t)(sqrt_x_prime * (1 << 23)) & 0x7FFFFF);

    float result;
    memcpy(&result, &result_bits, sizeof(result));

    return result;
}