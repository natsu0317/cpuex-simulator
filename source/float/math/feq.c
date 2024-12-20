#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

bool feq(float a, float b){
    const float epsilon = 1e-6;
    return fabs(a - b) < epsilon;
}