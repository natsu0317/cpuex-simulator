#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "./math/math_functions.h"

// レジスタ関連の関数（テスト用にモック）
void set_register(int rd, float value) {
    printf("レジスタ x%d に値 %.6f を設定\n", rd, value);
}

float get_register(int rs) {
    // テスト用にランダムな値を返す
    return ((float)rand() / RAND_MAX) * 100.0f - 50.0f;
}

// テスト用の補助関数
void print_test_header(const char* test_name) {
    printf("\n===== %s のテスト =====\n", test_name);
}

void check_result(float result, float expected, float tolerance) {
    float diff = fabsf(result - expected);
    if (diff <= tolerance || (isnan(result) && isnan(expected))) {
        printf("✅ 成功: 結果 %.9f, 期待値 %.9f, 差 %.9e\n", result, expected, diff);
    } else {
        printf("❌ 失敗: 結果 %.9f, 期待値 %.9f, 差 %.9e\n", result, expected, diff);
    }
}

void check_bool_result(bool result, bool expected) {
    if (result == expected) {
        printf("✅ 成功: 結果 %s, 期待値 %s\n", result ? "true" : "false", expected ? "true" : "false");
    } else {
        printf("❌ 失敗: 結果 %s, 期待値 %s\n", result ? "true" : "false", expected ? "true" : "false");
    }
}

void check_int_result(int32_t result, int32_t expected) {
    if (result == expected) {
        printf("✅ 成功: 結果 %d, 期待値 %d\n", result, expected);
    } else {
        printf("❌ 失敗: 結果 %d, 期待値 %d\n", result, expected);
    }
}

// 各関数のテスト
void test_fadd() {
    print_test_header("fadd");
    
    // 基本的なテストケース
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的な計算
        {1.0f, 2.0f, 3.0f},
        {-1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        
        // πを含む計算
        {3.141593f, 2.718282f, 5.859875f},  // π + e
        {3.141593f, -3.141593f, 0.0f},      // π + (-π)
        {3.141593f, 3.141593f, 6.283186f},  // π + π = 2π
        
        // 小数点以下の精度
        {0.333333f, 0.666667f, 1.0f},
        {0.1f, 0.2f, 0.3f},
        {0.123456f, 0.654321f, 0.777777f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fadd(test_cases[i].a, test_cases[i].b);
        printf("fadd(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fsub() {
    print_test_header("fsub");
    
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的な計算
        {3.0f, 2.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, -0.0f, 0.0f},
        
        // πを含む計算
        {6.283186f, 3.141593f, 3.141593f},  // 2π - π = π
        {3.141593f, 3.141593f, 0.0f},       // π - π = 0
        {3.141593f, 2.718282f, 0.423311f},  // π - e
        
        // 小数点以下の精度
        {1.0f, 0.333333f, 0.666667f},
        {0.3f, 0.1f, 0.2f},
        {0.777777f, 0.123456f, 0.654321f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsub(test_cases[i].a, test_cases[i].b);
        printf("fsub(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fmul() {
    print_test_header("fmul");
    
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的な計算
        {2.0f, 3.0f, 6.0f},
        {-2.0f, -3.0f, 6.0f},
        {-2.0f, 3.0f, -6.0f},
        {0.0f, 5.0f, 0.0f},
        
        // πを含む計算
        {3.141593f, 2.0f, 6.283186f},       // π * 2 = 2π
        {3.141593f, 3.141593f, 9.869604f},  // π * π = π²
        {3.141593f, 0.318310f, 1.0f},       // π * (1/π) ≈ 1
        
        // 小数点以下の精度
        {0.5f, 0.5f, 0.25f},
        {0.1f, 0.1f, 0.01f},
        {0.333333f, 3.0f, 1.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fmul(test_cases[i].a, test_cases[i].b);
        printf("fmul(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fdiv() {
    print_test_header("fdiv");
    
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的な計算
        {6.0f, 3.0f, 2.0f},
        {-6.0f, -3.0f, 2.0f},
        {-6.0f, 3.0f, -2.0f},
        {0.0f, 5.0f, 0.0f},
        
        // πを含む計算
        {6.283186f, 3.141593f, 2.0f},       // 2π / π = 2
        {3.141593f, 3.141593f, 1.0f},       // π / π = 1
        {1.0f, 3.141593f, 0.318310f},       // 1 / π ≈ 0.318
        
        // 小数点以下の精度
        {0.25f, 0.5f, 0.5f},
        {0.01f, 0.1f, 0.1f},
        {1.0f, 3.0f, 0.333333f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fdiv(test_cases[i].a, test_cases[i].b);
        printf("fdiv(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fsgnjn() {
    print_test_header("fsgnjn");
    
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 2.0f, -1.0f},     // 正 + 正 -> 負
        {1.0f, -2.0f, 1.0f},     // 正 + 負 -> 正
        {-1.0f, 2.0f, -1.0f},    // 負 + 正 -> 負
        {-1.0f, -2.0f, 1.0f},    // 負 + 負 -> 正
        
        // πを含む値
        {3.141593f, 2.0f, -3.141593f},      // π, 正 -> -π
        {3.141593f, -2.0f, 3.141593f},      // π, 負 -> π
        {-3.141593f, 2.0f, -3.141593f},     // -π, 正 -> -π
        {-3.141593f, -2.0f, 3.141593f},     // -π, 負 -> π
        
        // 特殊なケース
        {0.0f, 2.0f, -0.0f},     // 0 + 正 -> -0
        {0.0f, -2.0f, 0.0f},     // 0 + 負 -> 0
        {INFINITY, 2.0f, -INFINITY},        // ∞, 正 -> -∞
        {INFINITY, -2.0f, INFINITY},        // ∞, 負 -> ∞
        {NAN, 2.0f, -NAN},       // NaN, 正 -> -NaN (符号ビットのみ変更)
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsgnjn(test_cases[i].a, test_cases[i].b);
        printf("fsgnjn(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fsgnjx() {
    print_test_header("fsgnjx");
    
    struct {
        float a;
        float b;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 2.0f, 1.0f},      // 正 XOR 正 -> 正
        {1.0f, -2.0f, -1.0f},    // 正 XOR 負 -> 負
        {-1.0f, 2.0f, -1.0f},    // 負 XOR 正 -> 負
        {-1.0f, -2.0f, 1.0f},    // 負 XOR 負 -> 正
        
        // πを含む値
        {3.141593f, 2.0f, 3.141593f},       // π, 正 -> π
        {3.141593f, -2.0f, -3.141593f},     // π, 負 -> -π
        {-3.141593f, 2.0f, -3.141593f},     // -π, 正 -> -π
        {-3.141593f, -2.0f, 3.141593f},     // -π, 負 -> π
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsgnjx(test_cases[i].a, test_cases[i].b);
        printf("fsgnjx(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_finv() {
    print_test_header("finv");
    
    struct {
        float a;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {2.0f, 0.5f},
        {-2.0f, -0.5f},
        {0.5f, 2.0f},
        {-0.5f, -2.0f},
        {1.0f, 1.0f},
        {-1.0f, -1.0f},
        
        // πを含む値
        {3.141593f, 0.318310f},           // 1/π ≈ 0.318310
        {0.318310f, 3.141593f},           // 1/(1/π) ≈ π
        {-3.141593f, -0.318310f},         // 1/(-π) ≈ -0.318310
        
        // 小数点以下の精度
        {0.1f, 10.0f},
        {0.01f, 100.0f},
        {0.333333f, 3.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = finv(test_cases[i].a);
        printf("finv(%.9f) = ", test_cases[i].a);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_fsqrt() {
    print_test_header("fsqrt");
    
    struct {
        float a;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {4.0f, 2.0f},
        {9.0f, 3.0f},
        {16.0f, 4.0f},
        {25.0f, 5.0f},
        {1.0f, 1.0f},
        
        // πを含む値
        {3.141593f, 1.772454f},           // √π ≈ 1.772454
        {9.869604f, 3.141593f},           // √(π²) = π
        
        // 小数点以下の精度
        {0.25f, 0.5f},
        {0.01f, 0.1f},
        {0.0625f, 0.25f},
        
        // 非整数値
        {2.0f, 1.414214f},                // √2 ≈ 1.414214
        {3.0f, 1.732051f},                // √3 ≈ 1.732051
        {5.0f, 2.236068f},                // √5 ≈ 2.236068
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsqrt(test_cases[i].a);
        printf("fsqrt(%.9f) = ", test_cases[i].a);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_flt() {
    print_test_header("flt");
    
    struct {
        float a;
        float b;
        bool expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 2.0f, true},      // 1.0 < 2.0
        {2.0f, 1.0f, false},     // 2.0 < 1.0
        {1.0f, 1.0f, false},     // 1.0 < 1.0
        {-1.0f, 1.0f, true},     // -1.0 < 1.0
        {1.0f, -1.0f, false},    // 1.0 < -1.0
        
        // πを含む値
        {3.141593f, 3.141594f, true},      // π < π+ε
        {3.141593f, 3.141593f, false},     // π < π
        {3.141593f, 3.141592f, false},     // π < π-ε
        {-3.141593f, 3.141593f, true},     // -π < π
        
        // 小数点以下の精度
        {0.333333f, 0.333334f, true},
        {0.333333f, 0.333333f, false},
        {0.333333f, 0.333332f, false},
        
        // 特殊なケース
        {0.0f, 0.0f, false},     // 0.0 < 0.0
        {-0.0f, 0.0f, false},    // -0.0 < 0.0
        {0.0f, -0.0f, false},    // 0.0 < -0.0
        {-INFINITY, INFINITY, true}, // -∞ < ∞
        {INFINITY, -INFINITY, false}, // ∞ < -∞
        {INFINITY, INFINITY, false},  // ∞ < ∞
        {-INFINITY, -INFINITY, false}, // -∞ < -∞
        {NAN, 0.0f, false},      // NaN < 0.0
        {0.0f, NAN, false},      // 0.0 < NaN
        {NAN, NAN, false},       // NaN < NaN
        {NAN, INFINITY, false},  // NaN < ∞
        {INFINITY, NAN, false},  // ∞ < NaN
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        bool result = flt(test_cases[i].a, test_cases[i].b);
        printf("flt(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_bool_result(result, test_cases[i].expected);
    }
}

void test_feq() {
    print_test_header("feq");
    
    struct {
        float a;
        float b;
        bool expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 1.0f, true},      // 1.0 == 1.0
        {1.0f, 2.0f, false},     // 1.0 == 2.0
        {-1.0f, -1.0f, true},    // -1.0 == -1.0
        {-1.0f, 1.0f, false},    // -1.0 == 1.0
        
        // πを含む値
        {3.141593f, 3.141593f, true},      // π == π
        {3.141593f, 3.141594f, false},     // π == π+ε
        {3.141593f, 3.141592f, false},     // π == π-ε
        {-3.141593f, -3.141593f, true},    // -π == -π
        
        // 小数点以下の精度
        {0.333333f, 0.333333f, true},
        {0.333333f, 0.333334f, false},
        {0.333333f, 0.333332f, false},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        bool result = feq(test_cases[i].a, test_cases[i].b);
        printf("feq(%.9f, %.9f) = ", test_cases[i].a, test_cases[i].b);
        check_bool_result(result, test_cases[i].expected);
    }
}

void test_fcvtws() {
    print_test_header("fcvtws");
    
    struct {
        float a;
        int32_t expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 1},
        {2.0f, 2},
        {-1.0f, -1},
        {-2.0f, -2},
        {0.0f, 0},
        {-0.0f, 0},
        
        // πを含む値
        {3.141593f, 3},          // π → 3 (切り捨て)
        {-3.141593f, -3},        // -π → -3 (切り捨て)
        
        // 小数点以下の値（切り捨て）
        {0.9f, 0},
        {1.9f, 1},
        {-0.9f, 0},
        {-1.9f, -1},
        
        // 小数点以下の値（四捨五入ではない）
        {0.5f, 0},
        {1.5f, 1},
        {-0.5f, 0},
        {-1.5f, -1},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        int32_t result = fcvtws(test_cases[i].a);
        printf("fcvtws(%.9f) = ", test_cases[i].a);
        check_int_result(result, test_cases[i].expected);
    }
}

void test_fcvtsw() {
    print_test_header("fcvtsw");
    
    struct {
        int32_t a;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {1, 1.0f},
        {2, 2.0f},
        {-1, -1.0f},
        {-2, -2.0f},
        {0, 0.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fcvtsw(test_cases[i].a);
        printf("fcvtsw(%d) = ", test_cases[i].a);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

void test_floor() {
    print_test_header("floor");
    
    struct {
        float a;
        float expected;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 1.0f},
        {1.1f, 1.0f},
        {1.5f, 1.0f},
        {1.9f, 1.0f},
        {2.0f, 2.0f},
        
        // 負の値
        {-1.0f, -1.0f},
        {-1.1f, -2.0f},
        {-1.5f, -2.0f},
        {-1.9f, -2.0f},
        {-2.0f, -2.0f},
        
        // πを含む値
        {3.141593f, 3.0f},       // floor(π) = 3
        {-3.141593f, -4.0f},     // floor(-π) = -4
        
        // ゼロ付近
        {0.0f, 0.0f},
        {0.1f, 0.0f},
        {0.9f, 0.0f},
        {-0.0f, -0.0f},
        {-0.1f, -1.0f},
        {-0.9f, -1.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = floor(test_cases[i].a);
        printf("floor(%.9f) = ", test_cases[i].a);
        check_result(result, test_cases[i].expected, 1e-6f);
    }
}

// メイン関数
int main() {
    // 乱数初期化
    srand(time(NULL));
    
    printf("===== 浮動小数点演算関数のテスト =====\n");
    
    // 各関数のテスト実行
    test_fadd();
    test_fsub();
    test_fmul();
    test_fdiv();
    test_fsgnjn();
    test_fsgnjx();
    test_finv();
    test_fsqrt();
    test_flt();
    test_feq();
    test_fcvtws();
    test_fcvtsw();
    test_floor();
    
    printf("\n===== すべてのテストが完了しました =====\n");
    
    return 0;
} 