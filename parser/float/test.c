#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fadd(float a, float b);
float fsub(float a, float b);
float fmul(float a, float b);
float fdiv(float a, float b);

int compare_floats(float a, float b) {
    // 両方がNaNの場合は等しいとみなす
    if (isnan(a) && isnan(b)) return 1;
    
    // 両方が無限大の場合、符号を確認
    if (isinf(a) && isinf(b)) return (signbit(a) == signbit(b));
    
    // 相対誤差を使用して比較
    float rel_error = fabsf(a - b) / fmaxf(fabsf(a), fabsf(b));
    return rel_error < FLT_EPSILON;
}

// テストケース
void run_test(float a, float b, float expected, const char* test_name) {
    float result = fmul(a, b);
    if (compare_floats(result, expected)) {
        printf("%s: PASSED\n", test_name);
    } else {
        printf("%s: FAILED. Expected %f, got %f\n", test_name, expected, result);
    }
}

int main() {
    // 基本的な除算
    // run_test(0.75f, 0.375f, 0.28125f, "Basic division");
    // run_test(-1.5f, 1.8f, -2.7f, "Division with negative number");
    // run_test(-1.5f, 0.2f, -0.3f,"easy");
    // run_test(-1.5f, 2.0f, -3.0f,"easy");

    // // ゼロとの除算
    // run_test(0.0f, 5.0f, 0.0f, "Division with zero numerator");
    // run_test(5.0f, 1.0f, 5.0f, "Division by one");
    
    // // 特別なケース
    // run_test(1.0f, 0.0f, INFINITY, "Division by zero (positive)");
    // run_test(-1.0f, 0.0f, -INFINITY, "Division by zero (negative)");

    // // 無限大の除算
    // run_test(INFINITY, 2.0f, INFINITY, "Infinity divided by finite");
    // run_test(-INFINITY, 2.0f, -INFINITY, "Negative infinity divided by finite");
    // run_test(INFINITY, INFINITY, NAN, "Infinity divided by infinity");
    // run_test(-INFINITY, -INFINITY, NAN, "Negative infinity divided by negative infinity");

    // // NaNの除算
    // run_test(NAN, 1.0f, NAN, "NaN divided by finite");
    // run_test(1.0f, NAN, NAN, "Finite divided by NaN");

    return 0;
}