#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define NUM_INTERVALS 1024

float fadd(float a, float b);
float fmul(float a, float b);

typedef struct {
    float a[NUM_INTERVALS];
    float b[NUM_INTERVALS];
} BlockRAM;

BlockRAM ram;

void initBlockRAM() {
    for (int i = 0; i < NUM_INTERVALS; i++) {
        float x1 = 1.0f + (float)i / NUM_INTERVALS;
        float x2 = 1.0f + (float)(i + 1) / NUM_INTERVALS;
        float y1 = 1.0f / x1;
        float y2 = 1.0f / x2;
        
        ram.a[i] = (y2 - y1) / (x2 - x1);
        ram.b[i] = y1 - ram.a[i] * x1;
    }
}

// float finv(float a) {
//     // 入力を [1, 2) の範囲に正規化
//     uint32_t bits;
//     memcpy(&bits, &a, sizeof(float));
//     int exp = ((bits >> 23) & 0xFF) - 127;
//     uint32_t mantissa = (bits & 0x007FFFFF) | 0x00800000;
//     float x = (float)mantissa / (1 << 23);
    
//     // インデックスの計算
//     int index = (int)((x - 1.0f) * NUM_INTERVALS);
//     if (index < 0) index = 0;
//     if (index >= NUM_INTERVALS) index = NUM_INTERVALS - 1;

//     // 近似計算
//     float result = fadd(ram.b[index], fmul(ram.a[index],x));
    
//     // 結果を元のスケールに戻す
//     return ldexpf(result, -exp);
// }

// int main() {
//     initBlockRAM();

//     float test_values[] = {1.0f, 1.25f, 1.5f, 1.75f, 2.34f, 1.99f, 0.5f, 2.0f, 10.0f};
//     int num_tests = sizeof(test_values) / sizeof(float);

//     for (int i = 0; i < num_tests; i++) {
//         float x = test_values[i];
//         float approx = finv(x);
//         float actual = 1.0f / x;
//         printf("x = %.2f, Approximation = %.6f, Actual = %.6f, Error = %.6f\n", 
//                x, approx, actual, fabsf(approx - actual));
//     }

//     return 0;
// }