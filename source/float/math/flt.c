#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// 浮動小数点数を比較する関数
bool flt(float x1, float x2) {
    uint32_t x1_bits, x2_bits;
    memcpy(&x1_bits, &x1, sizeof(x1_bits));
    memcpy(&x2_bits, &x2, sizeof(x2_bits));

    // 符号、指数、仮数の抽出
    uint32_t s1 = (x1_bits >> 31) & 0x1;
    uint32_t e1 = (x1_bits >> 23) & 0xFF;
    uint32_t m1 = x1_bits & 0x7FFFFF;

    uint32_t s2 = (x2_bits >> 31) & 0x1;
    uint32_t e2 = (x2_bits >> 23) & 0xFF;
    uint32_t m2 = x2_bits & 0x7FFFFF;

    // どちらも0の場合
    if (x1 == 0 && x2 == 0) {
        return false;
    }

    // 符号が異なる場合
    if (s1 != s2) {
        return s1 > s2; // 負の数がある場合、負の数の方が小さい
    }

    // 両方が正の数の場合
    if (s1 == 0) {
        if (e1 == e2) {
            return m1 < m2;
        }
        return e1 < e2;
    }

    // 両方が負の数の場合
    if (e1 == e2) {
        return m1 > m2;
    }
    return e1 > e2;
}