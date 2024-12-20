#include <stdio.h>
#include <stdint.h>
#include <string.h>

// fsgnjxの動作を模倣する関数
float fsgnjx(float x1, float x2) {
    uint32_t x1_bits, x2_bits;
    memcpy(&x1_bits, &x1, sizeof(x1_bits));
    memcpy(&x2_bits, &x2, sizeof(x2_bits));

    // XOR演算で符号ビットを決定（ただし、ここではx1とx2が同じなので符号ビットは0になる）
    uint32_t result_bits = (x1_bits & 0x7FFFFFFF) | ((x1_bits ^ x2_bits) & 0x80000000);

    float result;
    memcpy(&result, &result_bits, sizeof(result));

    return result;
}