#include "fadd.c"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fsub(float a, float b){
    fadd(a, -b);
}