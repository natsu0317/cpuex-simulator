#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
#include <stdbool.h>
#include <stdint.h>

#define NUM_INTERVALS 1024

typedef struct {
    float a[NUM_INTERVALS];
    float b[NUM_INTERVALS];
} BlockRAM;

extern BlockRAM ram;
extern int ram_initialized;

float fadd(float a, float b);
float fsub(float a, float b);
float fmul(float a, float b);
float fdiv(float a, float b);
float fsqrt(float a);
float finv(float a);
float fabsf(float a);
float fcvtsw(int32_t x);
int32_t fcvtws(float x);
bool feq(float a, float b);
bool flt(float a, float b);
float fneg(float a);
float fsgnjn(float a, float b);
float fsgnjx(float a, float b);

void initBlockRAM();

#endif // MATH_FUNCTIONS_H