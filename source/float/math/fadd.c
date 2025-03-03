#include "math_functions.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

float fadd(float x1, float x2) {
    uint32_t x1_bits, x2_bits;
    memcpy(&x1_bits, &x1, sizeof(x1_bits));
    memcpy(&x2_bits, &x2, sizeof(x2_bits));

    // 符号、指数、仮数部に分解
    uint32_t s1 = (x1_bits >> 31) & 0x1;
    uint32_t e1 = (x1_bits >> 23) & 0xFF;
    uint32_t m1 = x1_bits & 0x7FFFFF;
    uint32_t s2 = (x2_bits >> 31) & 0x1;
    uint32_t e2 = (x2_bits >> 23) & 0xFF;
    uint32_t m2 = x2_bits & 0x7FFFFF;

    // 仮数部の拡張
    uint32_t m1a = (e1 == 0) ? m1 : (0x800000 | m1);
    uint32_t m2a = (e2 == 0) ? m2 : (0x800000 | m2);
    
    // 指数部の調整
    uint32_t e1a = (e1 == 0) ? 1 : e1;
    uint32_t e2a = (e2 == 0) ? 1 : e2;
    
    // 大小比較
    uint32_t ce = (e1a > e2a) ? 0 : 1;
    uint32_t tde = (e1a > e2a) ? e1a - e2a : e2a - e1a;
    
    // シフト量の決定
    uint32_t de = (tde > 31) ? 31 : tde;
    uint32_t sel;
    if (de == 0) {
        sel = (m1a > m2a) ? 0 : 1;
    } else {
        sel = ce;
    }
    
    // 大きい方と小さい方の決定
    uint32_t ms = (sel == 0) ? m1a : m2a;
    uint32_t mi = (sel == 0) ? m2a : m1a;
    uint32_t es = (sel == 0) ? e1a : e2a;
    // 変数eiは使用されていないため削除
    uint32_t ss = (sel == 0) ? s1 : s2;
    
    // 小さい方のシフト
    uint64_t mie = ((uint64_t)mi) << 31;
    uint64_t mia = mie >> de;
    
    // スティッキービットの計算
    uint32_t tstck = (mia & 0x1FFFFFFF) != 0;
    
    // 加減算
    uint32_t mye;
    if (s1 == s2) {
        mye = (ms << 2) + (mia >> 29);
    } else {
        mye = (ms << 2) - (mia >> 29);
    }
    
    // 指数部の調整
    uint32_t esi = es + 1;
    
    // 正規化
    uint32_t eyd, myd;
    uint32_t stck;
    if ((mye >> 26) & 0x1) {
        if (esi == 255) {
            eyd = 255;
            myd = 0x2000000; // 01 followed by 25 zeros
            stck = 0;
        } else {
            eyd = esi;
            myd = mye >> 1;
            stck = tstck || (mye & 0x1);
        }
    } else {
        eyd = es;
        myd = mye;
        stck = tstck;
    }
    
    // 先頭の1を探す
    uint32_t se = 0;
    for (int i = 25; i >= 0; i--) {
        if ((myd >> i) & 0x1) {
            se = 25 - i;
            break;
        }
    }
    
    // 指数部の調整
    int32_t eyf = eyd - se;
    uint32_t eyr = (eyf > 0) ? eyf : 0;
    
    // 仮数部のシフト
    uint32_t myf;
    if (eyf > 0) {
        myf = myd << se;
    } else {
        myf = myd << (eyd - 1);
    }
    
    // 丸め処理
    uint32_t myr;
    if (((myf & 0x2) && !(myf & 0x1) && !stck && (myf & 0x4)) ||
        ((myf & 0x2) && !(myf & 0x1) && (s1 == s2) && stck) ||
        ((myf & 0x2) && (myf & 0x1))) {
        myr = (myf >> 2) + 1;
    } else {
        myr = myf >> 2;
    }
    
    // 指数部の調整
    uint32_t eyri = eyr + 1;
    
    // 結果の組み立て
    uint32_t ey, my;
    if (myr & 0x1000000) {
        ey = eyri;
        my = 0;
    } else if ((myr & 0xFFFFFF) == 0) {
        ey = 0;
        my = 0;
    } else {
        ey = eyr;
        my = myr & 0x7FFFFF;
    }
    
    // 符号の決定
    uint32_t sy;
    if (ey == 0 && my == 0) {
        sy = s1 && s2;
    } else {
        sy = ss;
    }
    
    // 特殊なケースの処理
    uint32_t nzm1 = (m1 != 0);
    uint32_t nzm2 = (m2 != 0);
    uint32_t y_bits;
    
    if (e1 == 255 && e2 != 255) {
        y_bits = (s1 << 31) | (255 << 23) | (nzm1 << 22) | (m1 & 0x3FFFFF);
    } else if (e2 == 255 && e1 != 255) {
        y_bits = (s2 << 31) | (255 << 23) | (nzm2 << 22) | (m2 & 0x3FFFFF);
    } else if (e1 == 255 && e2 == 255 && nzm1) {
        y_bits = (s1 << 31) | (255 << 23) | (1 << 22) | (m1 & 0x3FFFFF);
    } else if (e1 == 255 && e2 == 255 && nzm2) {
        y_bits = (s2 << 31) | (255 << 23) | (1 << 22) | (m2 & 0x3FFFFF);
    } else if (e1 == 255 && e2 == 255 && s1 == s2) {
        y_bits = (s1 << 31) | (255 << 23) | 0;
    } else if (e1 == 255 && e2 == 255) {
        y_bits = (1 << 31) | (255 << 23) | (1 << 22) | 0;
    } else {
        y_bits = (sy << 31) | (ey << 23) | my;
    }
    
    // オーバーフローのチェック（コメントアウトして警告を回避）
    // オーバーフロー情報が必要な場合は、関数の引数として渡すか、
    // グローバル変数などで保持する方法を検討してください
    /*
    uint32_t ovf = 0;
    if ((((mye >> 26) & 0x1) && esi == 255) || 
        ((myr & 0x1000000) && eyri == 255 && e1 != 255 && e2 != 255)) {
        ovf = 1;
    }
    */
    
    float result;
    memcpy(&result, &y_bits, sizeof(result));
    return result;
}