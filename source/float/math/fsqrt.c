#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>

// IEEE 754 single-precision floating-point format
typedef union {
    float f;
    uint32_t i;
} FloatInt;

// Constants
#define SQRT_TABLE_SIZE 1024
#define SQRT_RANGE_MIN 1.0f
#define SQRT_RANGE_MAX 4.0f

// Pre-computed coefficients (a and b) for linear approximation
static float sqrt_coeffs[SQRT_TABLE_SIZE][2];
static bool table_initialized = false;

// Initialize the sqrt_coeffs table
static void init_sqrt_table() {
    for (int i = 0; i < SQRT_TABLE_SIZE; i++) {
        float x1 = SQRT_RANGE_MIN + (float)i * (SQRT_RANGE_MAX - SQRT_RANGE_MIN) / SQRT_TABLE_SIZE;
        float x2 = x1 + (SQRT_RANGE_MAX - SQRT_RANGE_MIN) / SQRT_TABLE_SIZE;
        float y1 = sqrtf(x1);
        float y2 = sqrtf(x2);
        float xm = (x1 + x2) / 2;
        float ym = sqrtf(xm);
        
        float a = (y2 - y1) / (x2 - x1);
        float b = ((y1 + y2) / 2 + ym) / 2 - a * xm;
        
        sqrt_coeffs[i][0] = a;
        sqrt_coeffs[i][1] = b;
    }
    table_initialized = true;
}

float fsqrt(float x) {
    if (!table_initialized) {
        init_sqrt_table();
    }

    if (x == 0.0f) return 0.0f;
    if (x < 0.0f) return NAN;  // Return NaN for negative inputs

    FloatInt fi;
    fi.f = x;

    int exp = ((fi.i >> 23) & 0xFF) - 127;
    fi.i = (fi.i & 0x007FFFFF) | 0x3F800000;
    
    while (fi.f >= 4.0f) {
        fi.f *= 0.25f;
        exp += 2;
    }

    int index = (int)((fi.f - SQRT_RANGE_MIN) * SQRT_TABLE_SIZE / (SQRT_RANGE_MAX - SQRT_RANGE_MIN));
    if (index >= SQRT_TABLE_SIZE) index = SQRT_TABLE_SIZE - 1;

    float a = sqrt_coeffs[index][0];
    float b = sqrt_coeffs[index][1];
    float result = a * fi.f + b;

    int new_exp = 127 + (exp >> 1);
    if (exp & 1) result *= 1.4142135623730950488f;  // sqrt(2)
    
    fi.f = result;
    fi.i = (fi.i & 0x007FFFFF) | (new_exp << 23);

    // float our_result = fi.f;
    // float lib_result = sqrtf(x);
    // float rel_error = fabsf(our_result - lib_result) / lib_result;

    // printf("%.2f\t%.6f\t%.6f\t%.2e\n", x, our_result, lib_result, rel_error);

    return fi.f;
}

// int main() {
//     printf("Testing fsqrt function:\n");
//     printf("x\tfsqrt(x)\tsqrt(x)\tRelative Error\n");

//     float max_error = 0.0f;
//     float total_error = 0.0f;
//     int num_tests = 0;

//     for (float x = 0.0f; x <= 100.0f; x += 0.1f) {
//         float our_result = fsqrt(x);
//         float lib_result = sqrtf(x);
//         float rel_error = fabsf(our_result - lib_result) / lib_result;

//         printf("%.2f\t%.6f\t%.6f\t%.2e\n", x, our_result, lib_result, rel_error);

//         max_error = fmaxf(max_error, rel_error);
//         total_error += rel_error;
//         num_tests++;
//     }

//     float avg_error = total_error / num_tests;

//     printf("\nError analysis:\n");
//     printf("Maximum relative error: %.2e\n", max_error);
//     printf("Average relative error: %.2e\n", avg_error);

//     return 0;
// }