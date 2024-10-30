#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

extern float fadd(float a, float b){
    uint32_t a_bits, b_bits;
    memcpy(&a_bits, &a, sizeof(a_bits));
    memcpy(&b_bits, &b, sizeof(b_bits));

    uint32_t s1 = (a_bits >> 31) & 0x1;
    uint32_t s2 = (b_bits >> 31) & 0x1;
    uint32_t e1 = (a_bits >> 23) & 0xFF;
    uint32_t e2 = (b_bits >> 23) & 0xFF;
    uint32_t m1 = a_bits & 0x7FFFFF;
    uint32_t m2 = b_bits & 0x7FFFFF;

    uint32_t s,e,m; // 出力
    printf("s1: %x, e1: %x, m1: %x\n",s1, e1, m1);
    printf("s2: %x, e2: %x, m2: %x\n",s2, e2, m2);

    //aとbの絶対値の大きさ比較
    int flag; // 0: a>b, 1: b>a
    int diff_e = 0; // 指数部の差

    uint32_t expand_m(uint32_t m, uint32_t e) {
        return (e == 0) ? m : (m | 0x800000);
    }

    m1 = expand_m(m1, e1) << 1; //計算で桁数が増減することを考慮(主に引き算)
    m2 = expand_m(m2, e2) << 1;
    
    printf("m1: %x, m2: %x\n",m1, m2);

    e1 = (e1 == 0) ? 1 : e1;
    e2 = (e2 == 0) ? 1 : e2;

    if(e1 > e2){
        flag = 0;
        diff_e = e1 - e2;
    } else if (e1 < e2){
        flag = 1;
        diff_e = e2 - e1;
    } else{
        flag = (m1 >= m2) ? 0 : 1;
    }
    printf("flag: %d,diff_e: %d\n",flag, diff_e);
    //シフト数は31に制限
    if(diff_e > 31){
        diff_e = 31;
    }

    //絶対値が小さいほうの仮数部を2数の指数部の差だけ右シフト
    if(flag == 0){
        m2 = m2 >> diff_e;
    } else {
        m1 = m1 >> diff_e;
    }
    printf("m1: %x, m2: %x\n",m1, m2);

    //符号部も合わせて実際に加算
    s = (flag == 0) ? s1 : s2;
    e = (flag == 0) ? e1 : e2;
    if(s1 == s2){
        m = m1 + m2;
    } else {
        if(flag == 0){
            m = m1 - m2;
        } else {
            m = m2 - m1;
        }
    }
    printf("s: %x, e: %x, m: %x\n",s,e,m);

    //演算結果が浮動小数点の表現となるようにシフトと指数部の調整
    int se = 26;
    for (int i = 25; i >= 0; i--) {
        if (m & (1 << i)) {
            se = 25 - i;
            break;
        }
    }
    if (se == 26) { // mが0の場合
        m = 0;
        e = 0;
    } else {
        e = e - se;
        m = m << se;
    }
    printf("m: %x\n",m);
    printf("se: %d,e: %x\n",se, e);
    
    // 丸め処理
    uint32_t stck = (m & ((1 << se) - 1)) != 0; // スティッキービットの計算
    printf("stck: %x\n",stck);

    // 偶数への丸め（round to even）
    if ((m & 0x1) && (((m & 0x2) != 0) || stck)) {
        m = (m >> 1) + 1;
    } else {
        m = m >> 1;
    }
    
    printf("m: %x\n",m);
    m = m >> 1; //31,32行目で左シフトした分を戻す
    printf("m: %x\n",m);
    // 正規化後の桁上がり処理
    if (m & 0x800000) {  // 24ビット目が1ならば
        m = m & 0x7fffff;
        printf("m: %x\n",m);
        e++;
    }

    // 非正規化数の処理
    if (e <= 0) {
        if (e < -23) {  // 非正規化数でも表現できない場合
            m = 0;
        } else {  // 非正規化数として表現
            m = m >> (1 - e);
            e = 0;
        }
    }

    // 正規化数の処理
    if (e >= 255) {  // オーバーフローの場合
        e = 255;
        m = 0;
    }

    // 結果の組み立て
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
    float result = fadd(a, b);
    if (compare_floats(result, expected)) {
        printf("%s: PASSED\n", test_name);
    } else {
        printf("%s: FAILED. Expected %f, got %f\n", test_name, expected, result);
    }
}

int main() {
    //run_test(0.375f, 0.125f, 0.5f, "test");
    // 基本的な加算
    //run_test(3.14f, 2.86f, 6.0f, "Basic addition");
    //run_test(-1.5f, 2.5f, 1.0f, "Addition with negative number");

    // // ゼロとの加算
    //run_test(0.0f, 5.0f, 5.0f, "Addition with zero");
    // run_test(-0.0f, 0.0f, 0.0f, "Addition of zeros");

    // // 大きな数と小さな数の加算
    //run_test(1e30f, 1.0f, 1e30f, "Large number + small number");

    // // オーバーフローのテスト
     //run_test(FLT_MAX, FLT_MAX, INFINITY, "Overflow test");

    // // アンダーフローのテスト
     //run_test(FLT_MIN / 2, FLT_MIN / 2, 0.0f, "Underflow test");

    // // 無限大のテスト
     //run_test(INFINITY, 5.0f, INFINITY, "Infinity + finite");
     //run_test(INFINITY, -INFINITY, NAN, "Infinity - Infinity");

    // // NaNのテスト
     //run_test(NAN, 1.0f, NAN, "NaN + finite");

    // // 丸め処理のテスト
     //run_test(16777215.0f, 1.0f, 16777216.0f, "Rounding test");

    // // 符号付きゼロのテスト
     //run_test(0.0f, -0.0f, 0.0f, "Signed zero test");

    return 0;

}
