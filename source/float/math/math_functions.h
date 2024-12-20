#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
#include <stdbool.h>
#include <stdint.h>

float fadd(float a, float b);
float fsub(float a, float b);
float fmul(float a, float b);
float fdiv(float a, float b);
float fsqrt(float a);
float finv(float a, float b);
float fabsf(float a);
float fcvtsw(int32_t x);
int32_t fcvtws(float x);
bool feq(float a, float b);
bool flt(float a, float b);
float fneg(float a);
float fsgnjn(float a, float b);
float fsgnjx(float a, float b);

#endif // MATH_FUNCTIONS_H