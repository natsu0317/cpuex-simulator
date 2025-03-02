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

// 各関数のテスト
void test_fadd() {
    print_test_header("fadd");
    
    // 基本的なテストケース
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的な計算
        // {1.0f, 2.0f},
        // {-1.0f, 1.0f},
        // {0.0f, 0.0f},
        
        // // πを含む計算
        {3.141593f, 2.718282f},  // π + e
        // {3.141593f, -3.141593f}, // π + (-π)
        // {3.141593f, 3.141593f},  // π + π = 2π
        
        // // 小数点以下の精度
        {0.333333f, 0.666667f},
        // {0.1f, 0.2f},
        {0.123456f, 0.654321f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fadd(test_cases[i].a, test_cases[i].b);
        printf("fadd(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_fsub() {
    print_test_header("fsub");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的な計算
        // {3.0f, 2.0f},
        // {1.0f, 1.0f},
        // {0.0f, -0.0f},
        
        // // πを含む計算
        // {6.283186f, 3.141593f},  // 2π - π = π
        // {3.141593f, 3.141593f},  // π - π = 0
        {3.141593f, 2.718282f},  // π - e
        
        // 小数点以下の精度
        // {1.0f, 0.333333f},
        // {0.3f, 0.1f},
        {0.777777f, 0.123456f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsub(test_cases[i].a, test_cases[i].b);
        printf("fsub(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_fmul() {
    print_test_header("fmul");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的な計算
        // {2.0f, 3.0f},
        // {-2.0f, -3.0f},
        // {-2.0f, 3.0f},
        // {0.0f, 5.0f},
        
        // // πを含む計算
        // {3.141593f, 2.0f},       // π * 2 = 2π
        // {3.141593f, 3.141593f},  // π * π = π²
        {3.141593f, 0.318310f},  // π * (1/π) ≈ 1
        
        // 小数点以下の精度
        // {0.5f, 0.5f},
        // {0.1f, 0.1f},
        // {0.333333f, 3.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fmul(test_cases[i].a, test_cases[i].b);
        printf("fmul(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_fdiv() {
    print_test_header("fdiv");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的な計算
        // {6.0f, 3.0f},
        // {-6.0f, -3.0f},
        // {-6.0f, 3.0f},
        // {0.0f, 5.0f},
        
        // // πを含む計算
        // {6.283186f, 3.141593f},  // 2π / π = 2
        // {3.141593f, 3.141593f},  // π / π = 1
        // {1.0f, 3.141593f},       // 1 / π ≈ 0.318
        
        // // 小数点以下の精度
        // {0.25f, 0.5f},
        {0.01f, 0.1f},
        // {1.0f, 3.0f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fdiv(test_cases[i].a, test_cases[i].b);
        printf("fdiv(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_fsgnjn() {
    print_test_header("fsgnjn");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的なケース
        // {1.0f, 2.0f},     // 正 + 正 -> 負
        // {1.0f, -2.0f},    // 正 + 負 -> 正
        // {-1.0f, 2.0f},    // 負 + 正 -> 負
        // {-1.0f, -2.0f},   // 負 + 負 -> 正
        
        // πを含む値
        // {3.141593f, 2.0f},      // π, 正 -> -π
        // {3.141593f, -2.0f},     // π, 負 -> π
        // {-3.141593f, 2.0f},     // -π, 正 -> -π
        // {-3.141593f, -2.0f},    // -π, 負 -> π
        
        // // 特殊なケース
        // {0.0f, 2.0f},     // 0 + 正 -> -0
        // {0.0f, -2.0f},    // 0 + 負 -> 0
        // {INFINITY, 2.0f},        // ∞, 正 -> -∞
        // {INFINITY, -2.0f},       // ∞, 負 -> ∞
        // {NAN, 2.0f},       // NaN, 正 -> -NaN (符号ビットのみ変更)
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsgnjn(test_cases[i].a, test_cases[i].b);
        printf("fsgnjn(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_fsgnjx() {
    print_test_header("fsgnjx");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的なケース
        // {1.0f, 2.0f},      // 正 XOR 正 -> 正
        {1.0f, -2.0f},    // 正 XOR 負 -> 負
        {-1.0f, 2.0f},    // 負 XOR 正 -> 負
        // {-1.0f, -2.0f},    // 負 XOR 負 -> 正
        
        // // πを含む値
        // {3.141593f, 2.0f},       // π, 正 -> π
        // {3.141593f, -2.0f},     // π, 負 -> -π
        // {-3.141593f, 2.0f},     // -π, 正 -> -π
        // {-3.141593f, -2.0f},     // -π, 負 -> π
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsgnjx(test_cases[i].a, test_cases[i].b);
        printf("fsgnjx(%.9f, %.9f) = %.9f\n", test_cases[i].a, test_cases[i].b, result);
    }
}

void test_finv() {
    print_test_header("finv");
    
    struct {
        float a;
    } test_cases[] = {
        // 基本的なケース
        // {2.0f},
        // {-2.0f},
        // {0.5f},
        // {-0.5f},
        // {1.0f},
        // {-1.0f},
        
        // // πを含む値
        // {3.141593f},           // 1/π ≈ 0.318310
        // {0.318310f},           // 1/(1/π) ≈ π
        // {-3.141593f},         // 1/(-π) ≈ -0.318310
        
        // // 小数点以下の精度
        // {0.1f},
        {0.01f},
        // {0.333333f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = finv(test_cases[i].a);
        printf("finv(%.9f) = %.9f\n", test_cases[i].a, result);
    }
}

void test_fsqrt() {
    print_test_header("fsqrt");
    
    struct {
        float a;
    } test_cases[] = {
        // 基本的なケース
        // {4.0f},
        {9.0f},
        {16.0f},
        {25.0f},
        // {1.0f},
        
        // // πを含む値
        // {3.141593f},           // √π ≈ 1.772454
        {9.869604f},           // √(π²) = π
        
        // 小数点以下の精度
        // {0.25f},
        {0.01f},
        {0.0625f},
        
        // 非整数値
        // {2.0f},                // √2 ≈ 1.414214
        // {3.0f},                // √3 ≈ 1.732051
        // {5.0f},                // √5 ≈ 2.236068
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fsqrt(test_cases[i].a);
        printf("fsqrt(%.9f) = %.9f\n", test_cases[i].a, result);
    }
}

void test_flt() {
    print_test_header("flt");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 2.0f},      // 1.0 < 2.0
        {2.0f, 1.0f},     // 2.0 < 1.0
        {1.0f, 1.0f},     // 1.0 < 1.0
        {-1.0f, 1.0f},     // -1.0 < 1.0
        {1.0f, -1.0f},    // 1.0 < -1.0
        
        // πを含む値
        {3.141593f, 3.141594f},      // π < π+ε
        {3.141593f, 3.141593f},     // π < π
        {3.141593f, 3.141592f},     // π < π-ε
        {-3.141593f, 3.141593f},     // -π < π
        
        // 小数点以下の精度
        {0.333333f, 0.333334f},
        {0.333333f, 0.333333f},
        {0.333333f, 0.333332f},
        
        // 特殊なケース
        {0.0f, 0.0f},     // 0.0 < 0.0
        {-0.0f, 0.0f},    // -0.0 < 0.0
        {0.0f, -0.0f},    // 0.0 < -0.0
        {-INFINITY, INFINITY}, // -∞ < ∞
        {INFINITY, -INFINITY}, // ∞ < -∞
        {INFINITY, INFINITY},  // ∞ < ∞
        {-INFINITY, -INFINITY}, // -∞ < -∞
        {NAN, 0.0f},      // NaN < 0.0
        {0.0f, NAN},      // 0.0 < NaN
        {NAN, NAN},       // NaN < NaN
        {NAN, INFINITY},  // NaN < ∞
        {INFINITY, NAN},  // ∞ < NaN
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        bool result = flt(test_cases[i].a, test_cases[i].b);
        printf("flt(%.9f, %.9f) = %s\n", test_cases[i].a, test_cases[i].b, result ? "true" : "false");
    }
}

void test_feq() {
    print_test_header("feq");
    
    struct {
        float a;
        float b;
    } test_cases[] = {
        // 基本的なケース
        {1.0f, 1.0f},      // 1.0 == 1.0
        {1.0f, 2.0f},     // 1.0 == 2.0
        {-1.0f, -1.0f},    // -1.0 == -1.0
        {-1.0f, 1.0f},    // -1.0 == 1.0
        
        // πを含む値
        {3.141593f, 3.141593f},      // π == π
        {3.141593f, 3.141594f},     // π == π+ε
        {3.141593f, 3.141592f},     // π == π-ε
        {-3.141593f, -3.141593f},    // -π == -π
        
        // 小数点以下の精度
        {0.333333f, 0.333333f},
        {0.333333f, 0.333334f},
        {0.333333f, 0.333332f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        bool result = feq(test_cases[i].a, test_cases[i].b);
        printf("feq(%.9f, %.9f) = %s\n", test_cases[i].a, test_cases[i].b, result ? "true" : "false");
    }
}

void test_fcvtws() {
    print_test_header("fcvtws");
    
    struct {
        float a;
    } test_cases[] = {
        // 基本的なケース
        {1.0f},
        {2.0f},
        {-1.0f},
        {-2.0f},
        {0.0f},
        {-0.0f},
        
        // πを含む値
        {3.141593f},          // π → 3 (切り捨て)
        {-3.141593f},        // -π → -3 (切り捨て)
        
        // 小数点以下の値（切り捨て）
        {0.9f},
        {1.9f},
        {-0.9f},
        {-1.9f},
        
        // 小数点以下の値（四捨五入ではない）
        {0.5f},
        {1.5f},
        {-0.5f},
        {-1.5f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        int32_t result = fcvtws(test_cases[i].a);
        printf("fcvtws(%.9f) = %d\n", test_cases[i].a, result);
    }
}

void test_fcvtsw() {
    print_test_header("fcvtsw");
    
    struct {
        int32_t a;
    } test_cases[] = {
        // 基本的なケース
        {1},
        {2},
        {-1},
        {-2},
        {0},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = fcvtsw(test_cases[i].a);
        printf("fcvtsw(%d) = %.9f\n", test_cases[i].a, result);
    }
}

void test_floor() {
    print_test_header("floor");
    
    struct {
        float a;
    } test_cases[] = {
        // 基本的なケース
        {1.0f},
        {1.1f},
        {1.5f},
        {1.9f},
        {2.0f},
        
        // 負の値
        {-1.0f},
        {-1.1f},
        {-1.5f},
        {-1.9f},
        {-2.0f},
        
        // πを含む値
        {3.141593f},       // floor(π) = 3
        {-3.141593f},     // floor(-π) = -4
        
        // ゼロ付近
        {0.0f},
        {0.1f},
        {0.9f},
        {-0.0f},
        {-0.1f},
        {-0.9f},
    };
    
    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        float result = floor(test_cases[i].a);
        printf("floor(%.9f) = %.9f\n", test_cases[i].a, result);
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
    // test_flt();
    // test_feq();
    // test_fcvtws();
    // test_fcvtsw();
    // test_floor();
    
    printf("\n===== すべてのテストが完了しました =====\n");
    
    return 0;
}