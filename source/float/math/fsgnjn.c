#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// fsgnjnの動作を模倣する関数
float fsgnjn(float x1, float x2) {
    uint32_t x1_bits, x2_bits;
    memcpy(&x1_bits, &x1, sizeof(x1_bits));
    memcpy(&x2_bits, &x2, sizeof(x2_bits));

    // 符号ビットを反転
    uint32_t result_bits = x1_bits ^ 0x80000000;

    float result;
    memcpy(&result, &result_bits, sizeof(result));

    return result;
}