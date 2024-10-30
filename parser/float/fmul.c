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
    printf("s1: %x, e1: %x, m1: %x\n",s1, e1, m1);
    printf("s2: %x, e2: %x, m2: %x\n",s2, e2, m2);

    e1 = (e1 == 0) ? 1 : e1;
    e2 = (e2 == 0) ? 1 : e2;

    uint32_t h1 = (m1 >> 11) & 0xFFF;
    uint32_t h2 = (m2 >> 11) & 0xFFF;
    uint32_t l1 = m1 & 0x7FF;
    uint32_t l2 = m2 & 0x7FF;
    printf("h1: %x, h2: %x, l1: %x, l2: %x\n",h1, h2, l1, l2);
    //ケチビットを補う
    h1 += 0x1000;
    h2 += 0x1000;
    printf("h1: %x, h2: %x\n",h1, h2);

    uint32_t HH = h1 * h2;
    uint32_t HL = h1 * l2;
    uint32_t LH = l1 * h2;
    printf("HH:%x,HL:%x,LH:%x\n",HH,HL,LH);

    m = HH + (HL >> 11) + (LH >> 11) + 2;
    printf("m:%x\n",m);

    //丸めは行わず最上位の1から下23bitを答えの仮数部
    int se;
    for(int i = 31; i >= 0; i--){
        if((( m >> i ) & 0x1) == 1 ){
            se = i;
            break;
        }
    }
    m = (m >> (se - 23)) & 0x7FFFFF;
    printf("m: %x\n",m);

    e = e1 + e2 - 127;
    s = (s1 == s2) ? 0 : 1;

    uint32_t result_bits = (s << 31) | (e << 23) | (m & 0x7FFFFF);
    float result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;

}

int compare_floats(float a, float b) {
    // 両方がNaNの場合は等しいとみなす
    if (isnan(a) && isnan(b)) return 1;
    
    // 両方が無限大の場合、符号を確認
    if (isinf(a) && isinf(b)) return (signbit(a) == signbit(b));
    
    // 相対誤差を使用して比較
    float rel_error = fabsf(a - b) / fmaxf(fabsf(a), fabsf(b));
    return rel_error < FLT_EPSILON;
}

// テストケース
void run_test(float a, float b, float expected, const char* test_name) {
    float result = fmul(a, b);
    if (compare_floats(result, expected)) {
        printf("%s: PASSED\n", test_name);
    } else {
        printf("%s: FAILED. Expected %f, got %f\n", test_name, expected, result);
    }
}

int main() {
    // 基本的な乗算
    run_test(3.0f, 2.0f, 6.0f, "Basic multiplication");
    run_test(-1.5f, 2.0f, -3.0f, "Multiplication with negative number");

    // ゼロとの乗算
    run_test(0.0f, 5.0f, 0.0f, "Multiplication with zero");
    run_test(-0.0f, 0.0f, 0.0f, "Multiplication of zeros");

    // 大きな数と小さな数の乗算
    run_test(1e30f, 1.0f, 1e30f, "Large number * small number");

    // オーバーフローのテスト
    run_test(FLT_MAX, 2.0f, INFINITY, "Overflow test");

    // アンダーフローのテスト
    run_test(FLT_MIN, FLT_MIN, 0.0f, "Underflow test");

    // 無限大のテスト
    run_test(INFINITY, 5.0f, INFINITY, "Infinity * finite");
    run_test(INFINITY, -INFINITY, -INFINITY, "Infinity * -Infinity");

    // NaNのテスト
    run_test(NAN, 1.0f, NAN, "NaN * finite");

    // 丸め処理のテスト
    run_test(16777215.0f, 1.0000001f, 16777215.0f, "Rounding test");

    // 符号付きゼロのテスト
    run_test(0.0f, -0.0f, -0.0f, "Signed zero test");

    return 0;
}