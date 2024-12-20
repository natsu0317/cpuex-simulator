#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fmul(float a, float b){
    uint32_t a_bits, b_bits;
    memcpy(&a_bits, &a, sizeof(a_bits));
    memcpy(&b_bits, &b, sizeof(b_bits));

    uint32_t s1 = (a_bits >> 31) & 0x1;
    uint32_t s2 = (b_bits >> 31) & 0x1;
    uint32_t e1 = (a_bits >> 23) & 0xFF;
    uint32_t e2 = (b_bits >> 23) & 0xFF;
    uint32_t m1 = a_bits & 0x7FFFFF;
    uint32_t m2 = b_bits & 0x7FFFFF;

    if(e1 == 0 && m1 == 0 || e2 == 0 && m2 == 0){
        return 0.0f;
    }

    uint32_t s,e,m; // 出力
    //printf("s1: %x, e1: %x, m1: %x\n",s1, e1, m1);
    //printf("s2: %x, e2: %x, m2: %x\n",s2, e2, m2);

    e1 = (e1 == 0) ? 1 : e1;
    e2 = (e2 == 0) ? 1 : e2;

    uint32_t h1 = (m1 >> 11) & 0xFFF;
    uint32_t h2 = (m2 >> 11) & 0xFFF;
    uint32_t l1 = m1 & 0x7FF;
    uint32_t l2 = m2 & 0x7FF;
    //printf("h1: %x, h2: %x, l1: %x, l2: %x\n",h1, h2, l1, l2);
    //ケチビットを補う
    h1 += 0x1000;
    h2 += 0x1000;
    //printf("h1: %x, h2: %x\n",h1, h2);

    uint32_t HH = h1 * h2;
    uint32_t HL = h1 * l2;
    uint32_t LH = l1 * h2;
    //printf("HH:%x,HL:%x,LH:%x\n",HH,HL,LH);

    m = HH + (HL >> 11) + (LH >> 11) + 2;
    //printf("m:%x\n",m);

    //丸めは行わず最上位の1から下23bitを答えの仮数部
    int se;
    for(int i = 31; i >= 0; i--){
        if((( m >> i ) & 0x1) == 1 ){
            se = i;
            break;
        }
    }
    m = (m >> (se - 23)) & 0x7FFFFF;
    //printf("m: %x\n",m);
    //printf("se: %d\n",se);

    e = e1 + e2 - 127 + (se - 24);
    s = (s1 == s2) ? 0 : 1;

    uint32_t result_bits = (s << 31) | (e << 23) | (m & 0x7FFFFF);
    float result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;

}
