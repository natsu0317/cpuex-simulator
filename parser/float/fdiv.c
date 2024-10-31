#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

float fadd(float a, float b);
float fmul(float a, float b);

float a[1024];//傾き
float b[1024];//絶対値

void init_ab(){
    float e = 1.0f / 1024.0f;
    for(int i=0; i<1024; i++){
        float x1 = 1.0f + i * e;
        float x2 = 1.0f + (i+1) * e;
        float x3 = 1.0f + (i + 0.5f) * e;
        a[i] = (1.0f / x2 - 1.0f / x1) / e;
        b[i] = 1.0f / x3 - a[i] * x3; 
    }
}

float calculate_inv(float x){
    uint32_t x_bits;
    memcpy(&x_bits, &x, sizeof(x));

    printf("x: %f\n",x);
    uint32_t index = (x_bits >> 13) & 0x3FF;
    printf("index: %x\n",index);
    float a_val = a[index];
    float b_val = b[index];
    printf("a_val: %f, b_val: %f\n",a_val,b_val);
    float result = fadd(*b, fmul(*a, x));
    return result;
}

float fdiv(float a, float b){
    printf("a: %f, b: %f\n",a,b);
    uint32_t a_bits, b_bits;
    memcpy(&a_bits, &a, sizeof(a_bits));
    memcpy(&b_bits, &b, sizeof(b_bits));

    uint32_t s1 = (a_bits >> 31) & 0x1;
    uint32_t s2 = (b_bits >> 31) & 0x1;
    uint32_t e1 = (a_bits >> 23) & 0xFF;
    uint32_t e2 = (b_bits >> 23) & 0xFF;
    uint32_t m1 = a_bits & 0x7FFFFF;
    uint32_t m2 = b_bits & 0x7FFFFF;
    
    printf("s1: %x, e1: %x, m1: %x\n",s1, e1, m1);
    printf("s2: %x, e2: %x, m2: %x\n",s2, e2, m2);

    // 片方でも0なら0を返す
    if (e1 == 0 && m1 == 0 || e2 == 0 && m2 == 0) {
        return 0.0f;
    }

    //ケチビットを補う
    m1 += 0x800000;
    m2 += 0x800000;

    uint32_t s,e,m; // 出力
    printf("s1: %x, e1: %x, m1: %x\n",s1, e1, m1);
    printf("s2: %x, e2: %x, m2: %x\n",s2, e2, m2);
    uint32_t e_m = 0x7F;
    init_ab();
    printf("m2_&0x4FFFFF: %x\n",m2 & 0x7FFFFF);
    uint32_t m2_for_inv_bits = (0 << 31) | (e_m << 23) | m2 & 0x7FFFFF;
    printf("m2_for_inv_bits: %x\n",m2_for_inv_bits);
    float m2_for_inv;
    memcpy(&m2_for_inv, &m2_for_inv_bits, sizeof(m2_for_inv));
    printf("m2_for_inv: %f\n",m2_for_inv);
    float m2_inv = calculate_inv(m2_for_inv);
    uint32_t m2_inv_bits;
    printf("m2_inv_bits: %x\n",m2_inv_bits);
    memcpy(&m2_inv_bits, &m2_inv, sizeof(m2_inv));
    printf("m2_inv: %f\n",m2_inv);

    uint32_t m1_m = (s1 << 31) | (e_m << 23) | m1 & 0x7FFFFF;
    uint32_t m2_m = (s2 << 31) | (e_m << 23) | m2_inv_bits & 0x7FFFFF;
    float m1_m_f, m2_m_f;
    memcpy(&m1_m_f, &m1_m, sizeof(m1_m_f));
    memcpy(&m2_m_f, &m2_m, sizeof(m2_m_f));
    printf("m1_m: %f\n",m1_m_f);
    printf("m2_m: %f\n",m2_m_f);
    printf("m1_m: %x\n",m1_m);
    printf("m2_m: %x\n",m2_m);
    float m_mul_f = fmul(m1_m_f, m2_m_f);
    uint32_t m_mul;
    memcpy(&m_mul, &m_mul_f, sizeof(m_mul));
    printf("m_mul: %f\n",m_mul_f);
    printf("m_mul; %x\n",m_mul);
    m = m_mul & 0x7FFFFF;
    printf("m: %x\n",m);

    s = (s1 == s2) ? 0 : 1;

    e = e1 - e2 + 127;
    printf("e: %x\n",e);

    // int se;
    // for(int i = 31; i >= 0; i--){
    //     if((( m >> i ) & 0x1) == 1 ){
    //         se = i;
    //         break;
    //     }
    // }
    // printf("se: %d\n",se);
    // m = (m >> (se - 23)) & 0x7FFFFF;
    // printf("m: %x\n",m);

    uint32_t result_bits = (s << 31) | (e << 23) | (m & 0x7FFFFF);
    float result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;

}
