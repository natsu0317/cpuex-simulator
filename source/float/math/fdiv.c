#include "math_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

float fadd(float a, float b);
float fsub(float a, float b);
float fmul(float a, float b);
float finv(float a);

float fdiv(float a, float b){
    return fmul(a, finv(b));
}

// int main(){
//     float a = 1.23f;
//     float b = 2.34f;
//     printf("finv(%f):%f\n",b,finv(b));
//     float result = fdiv(a,b);
//     printf("Result of fdiv(%f, %f) = %f\n", a, b, result);
//     return 0;
// }