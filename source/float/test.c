#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "math/math_function.h"

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

void execute_and_write_result(const char* operation, float a, float b, FILE* output_file) {
    float result;
    if (strcmp(operation, "fadd") == 0) {
        result = fadd(a, b);
    } else if (strcmp(operation, "fsub") == 0) {
        result = fsub(a, b);
    } else if (strcmp(operation, "fmul") == 0) {
        result = fmul(a, b);
    } else if (strcmp(operation, "fdiv") == 0) {
        result = fdiv(a, b);
    } else {
        fprintf(stderr, "Unknown operation: %s\n", operation);
        return;
    }
    fprintf(output_file, "%s(%f, %f) = %f\n", operation, a, b, result);
}

int main() {
    FILE *file;
    char line[128];

    file = fopen("flu.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Error opening file");
        fclose(file);
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        char operation[16];
        float a, b;
        if (sscanf(line, "%15s %f %f", operation, &a, &b) == 3) {
            execute_and_write_result(operation, a, b, output_file);
        } else {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    fclose(file);
    fclose(output_file);

    return 0;
}