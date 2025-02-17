#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define NUM_INTERVALS 1024

typedef struct {
    float a[NUM_INTERVALS];
    float b[NUM_INTERVALS];
} BlockRAM;

static BlockRAM ram;
static int ram_initialized = 0;

// BlockRAM の初期化関数
static void initBlockRAM() {
    for (int i = 0; i < NUM_INTERVALS; i++) {
        float x1 = 1.0f + (float)i * 3.0f / NUM_INTERVALS;
        float x2 = 1.0f + (float)(i + 1) * 3.0f / NUM_INTERVALS;
        float y1 = sqrtf(x1); 
        float y2 = sqrtf(x2); 
        float xmid = (x1 + x2) / 2.0f;
        float ymid = sqrtf(xmid);  
        float yavg = (y1 + y2) / 2.0f;
        
        ram.a[i] = (y2 - y1) / (x2 - x1);
        ram.b[i] = (ymid + yavg) / 2.0f - ram.a[i] * xmid;
    }
    ram_initialized = 1;
}

// 2の累乗を計算する関数
float pow2(int exp) {
    uint32_t result = 1 << (exp + 127);
    float f;
    memcpy(&f, &result, sizeof(float));
    return f;
}

float fsqrt(float x) {
    if (x == 0.0f) return 0.0f;

    if (!ram_initialized) {
        initBlockRAM();
    }

    uint32_t bits;
    memcpy(&bits, &x, sizeof(float));
    int exp = ((bits >> 23) & 0xFF) - 127;
    int exp_adj = exp / 2;
    uint32_t mantissa = (bits & 0x007FFFFF) | 0x00800000;
    float x_norm = (float)mantissa / (1 << 23);
    if (exp % 2) x_norm *= 2.0f;

    int index = (int)((x_norm - 1.0f) * NUM_INTERVALS / 3.0f);
    if (index < 0) index = 0;
    if (index >= NUM_INTERVALS) index = NUM_INTERVALS - 1;

    float result = ram.a[index] * x_norm + ram.b[index];

    // powfの代わりに自作の pow2 関数を使用
    result *= pow2(exp_adj);

    return result;
}

// int main() {
//     float test_values[] = {0.0f, 1.0f, 2.0f, 4.0f, 9.0f, 16.0f, 25.0f, 100.0f};
//     int num_tests = sizeof(test_values) / sizeof(float);

//     for (int i = 0; i < num_tests; i++) {
//         float x = test_values[i];
//         float approx = fsqrt(x);
//         printf("sqrt(%f) = %f (Approximation)\n", x, approx);
//     }

//     return 0;
// }