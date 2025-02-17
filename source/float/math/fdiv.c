#include "math_functions.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

BlockRAM ram;
int ram_initialized = 0;

void initBlockRAM() {
    for (int i = 0; i < NUM_INTERVALS; i++) {
        float x1 = 1.0f + (float)i / NUM_INTERVALS;
        float x2 = 1.0f + (float)(i + 1) / NUM_INTERVALS;
        float y1 = 1.0f / x1;
        float y2 = 1.0f / x2;
        
        ram.a[i] = (y2 - y1) / (x2 - x1);
        ram.b[i] = y1 - ram.a[i] * x1;
    }
    ram_initialized = 1;
}

float finv(float a) {
    // 初めて呼び出されたときにのみ初期化を行う
    if (!ram_initialized) {
        initBlockRAM();
    }

    // 入力の符号を保存
    int sign = (*(uint32_t*)&a) & 0x80000000;

    // 絶対値を取る
    a = fabsf(a);

    // 入力を [1, 2) の範囲に正規化
    uint32_t bits;
    memcpy(&bits, &a, sizeof(float));
    int exp = ((bits >> 23) & 0xFF) - 127;
    uint32_t mantissa = (bits & 0x007FFFFF) | 0x00800000;
    float x = (float)mantissa / (1 << 23);
    
    // インデックスの計算
    int index = (int)((x - 1.0f) * NUM_INTERVALS);
    if (index < 0) index = 0;
    if (index >= NUM_INTERVALS) index = NUM_INTERVALS - 1;

    // 近似計算
    float result = ram.a[index] * x + ram.b[index];
    
    // 結果を元のスケールに戻す
    result = ldexpf(result, -exp);

    // 元の符号を復元
    *(uint32_t*)&result ^= sign;

    return result;
}

float fdiv(float a, float b){
    float result = fmul(a, finv(b));
    // printf("%f / %f = %f error = %e\n", a, b, result, a/b - result);
    return result;
}