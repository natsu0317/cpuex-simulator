#include "math_functions.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>

extern float fsub(float a, float b){
    return fadd(a, -b);
}