#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fneg(float x){
    return fsgnjn(x,x);
}