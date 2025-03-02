#include "math_functions.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

BlockRAM ram;
int ram_initialized = 0;

void initBlockRAM() {
    if (ram_initialized) return;
    
    for (int i = 0; i < NUM_INTERVALS; i++) {
        /* グラフの傾きの絶対値 */
        double a = 1024.0 * (1024.0 / (1024.0 + (double)i) - 1024.0 / (1025.0 + (double)i));
        /* グラフの切片 */
        double b = 1024.0 * (1.0 - (1024.0+(double)i)/(1025.0+(double)i)) + (768.0 / (1024.0 + (double)i) - 256.0 / (1025.0 + (double)i) + 1024.0 / (2049.0 + (double)(2 * i)));

        ram.a[i] = (float)a;
        ram.b[i] = (float)b;
    }
    
    ram_initialized = 1;
}

float finv(float x) {  // 引数名をaからxに変更
    // 初めて呼び出されたときにのみ初期化を行う
    if (!ram_initialized) {
        initBlockRAM();
    }

    uint32_t bits;
    memcpy(&bits, &x, sizeof(float));
    
    uint32_t s = (bits >> 31) & 0x1;
    uint32_t e = (bits >> 23) & 0xFF;
    uint32_t m = bits & 0x7FFFFF;
    
    // キーの計算（仮数部の上位10ビット）
    uint32_t key = (m >> 13) & 0x3FF;
    // 近似計算
    float a_val = ram.a[key];  // 変数名をaからa_valに変更
    float b_val = ram.b[key];  // 変数名をbからb_valに変更

    // 正規化された入力の作成
    uint32_t x_normalized_bits = (127 << 23) | m;
    float x_normalized;
    memcpy(&x_normalized, &x_normalized_bits, sizeof(float));
    
    // a * x_normalized の計算
    float ax = a_val * x_normalized;  // a_valを使用
    
    // b - ax の計算
    float my = b_val - ax;  // b_valを使用
    
    // 指数部の計算: e_x + e_inv(x) - 127 = 126
    uint32_t ey = (e == 0) ? 0 : 253 - e;
    
    // 結果の組み立て
    uint32_t my_bits;
    memcpy(&my_bits, &my, sizeof(uint32_t));
    
    uint32_t result_bits = (s << 31) | (ey << 23) | (my_bits & 0x7FFFFF);
    float result;
    memcpy(&result, &result_bits, sizeof(float));

    return result;
}

float fdiv(float x1, float x2) {
    // 符号、指数、仮数部に分解
    uint32_t bits1, bits2;
    memcpy(&bits1, &x1, sizeof(float));
    memcpy(&bits2, &x2, sizeof(float));
    
    uint32_t s1 = (bits1 >> 31) & 0x1;
    uint32_t s2 = (bits2 >> 31) & 0x1;
    uint32_t e1 = (bits1 >> 23) & 0xFF;
    uint32_t e2 = (bits2 >> 23) & 0xFF;
    uint32_t m1 = bits1 & 0x7FFFFF;
    uint32_t m2 = bits2 & 0x7FFFFF;
    
    // m2_n = {1'b0, 8'd255, m2} = 1.{m2}の正規化表現
    uint32_t m2_n = (0xFF << 23) | m2;
    float m2_n_float;
    memcpy(&m2_n_float, &m2_n, sizeof(float));
    
    // m2_n_inv = 1 / 1.{m2} = 1.{m3}
    float m2_n_inv_float = finv(m2_n_float);
    uint32_t m2_n_inv;
    memcpy(&m2_n_inv, &m2_n_inv_float, sizeof(uint32_t));
    
    // 仮数部の乗算: 1.{m1} * 1.{m3}
    uint32_t m1_extended = m1 | 0x800000; // 暗黙の1ビットを追加
    uint32_t m3 = m2_n_inv & 0x7FFFFF;
    uint32_t m3_extended = m3 | 0x800000; // 暗黙の1ビットを追加
    
    // 26ビットの結果を得るための乗算
    uint64_t my1_64 = (uint64_t)m1_extended * (uint64_t)m3_extended;
    uint32_t my1 = (uint32_t)(my1_64 >> 23); // 上位26ビットを取得
    
    // キャリーの確認と仮数部の調整
    uint32_t carry = (my1 >> 25) & 0x1;
    uint32_t my2;
    if (carry) {
        my2 = (my1 >> 2) & 0x7FFFFF; // my1[24:2]
    } else {
        my2 = (my1 >> 1) & 0x7FFFFF; // my1[23:1]
    }
    
    // 指数部の計算
    int32_t ey1 = (int32_t)e1 - (int32_t)e2 + 126;
    int32_t ey2 = ey1 + 1;
    int32_t ey3 = carry ? ey2 : ey1;
    
    // アンダーフローとオーバーフローの確認
    uint32_t udf = (e1 == 0 || e2 == 255 || ey3 < 0 || ey3 == 0);
    uint32_t ovf = (e1 == 255 || e2 == 0 || ey3 > 254 || ey3 == 255);
    
    // 結果の組み立て
    uint32_t sy = s1 ^ s2;
    uint32_t ey = udf ? 0 : (ovf ? 255 : (uint32_t)ey3);
    uint32_t my = (udf || ovf) ? 0 : my2;
    
    uint32_t result_bits = (sy << 31) | (ey << 23) | my;
    float result;
    memcpy(&result, &result_bits, sizeof(float));
    
    return result;
}
