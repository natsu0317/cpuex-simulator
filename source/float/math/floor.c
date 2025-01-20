#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <float.h>

int32_t fcvtws(float a);
float fcvtsw(int32_t x);

float floorf(float a){
    int32_t int_val = fcvtws(a);
    float float_val = fcvtsw(int_val);

    if(float_val > a){
        return float_val - 1.0f;
    } else {
        return float_val;
    }
}

// テスト関数
void test_process_float() {
    float test_values[] = {3.75f, -2.5f, 0.0f, 123.456f, -789.123f};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++) {
        float original = test_values[i];
        float result = floorf(original);
        printf("Original: %.7g, Processed: %.7g\n", original, result);
    }
}

// // メイン関数
// int main() {
//     printf("Testing process_float:\n");
//     test_process_float();
//     return 0;
// }