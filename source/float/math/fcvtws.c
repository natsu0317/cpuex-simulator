#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

int32_t fcvtws(float x) {
    // 特殊ケース：入力が0の場合
    if (x == 0.0f) {
        return 0;
    }

    uint32_t x_bits;
    memcpy(&x_bits, &x, sizeof(x_bits));

    uint32_t sign = (x_bits >> 31) & 0x1;
    uint32_t exponent = (x_bits >> 23) & 0xFF;
    uint32_t mantissa = x_bits & 0x7FFFFF;

    // ケチビットを追加して31ビットに拡張
    uint32_t m = (mantissa | 0x800000) << 8;

    // 指数部のバイアスを考慮
    int32_t e = (int32_t)exponent - 127;

    if (e < 0) {
        // 指数が負の場合、整数部分は0
        return 0;
    } else if (e > 31) {
        // オーバーフロー
        return sign ? INT32_MIN : INT32_MAX;
    }

    // ビットシフト
    uint32_t result = m >> (23 - e);

    // 丸め処理
    if (m & (1 << (22 - e))) {
        result++;
    }

    // 符号の復元
    if (sign) {
        result = -result;
    }

    return (int32_t)result;
}
