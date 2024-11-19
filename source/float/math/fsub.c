#include "math_functions.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fadd(float a, float b);

extern float fsub(float a, float b){
    fadd(a, -b);
}