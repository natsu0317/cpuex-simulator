#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "fpu.hpp"

using namespace std;

// FP32の構造体
struct FP32 {
    uint32_t raw; // 生データ
    // 符号ビットを取得
    bool sign() const { return (raw >> 31) & 1; }
    // 指数部を取得
    uint8_t exponent() const { return (raw >> 23) & 0xFF; }
    // 仮数部を取得
    uint32_t mantissa() const { return raw & 0x7FFFFF; }
    // 正規化された仮数部 (暗黙の1を含む)
    uint32_t normalized_mantissa() const { return mantissa() | 0x800000; }
};

float fadd(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    int32_t m1a, m2a;
    if (x1.exponent() == 0) {
        m1a = x1.mantissa();
    } else {
        m1a = x1.normalized_mantissa();
    }
    if (x2.exponent() == 0) {
        m2a = x2.mantissa();
    } else {
        m2a = x2.normalized_mantissa();
    }
    uint32_t e1a, e2a;
    e1a = x1.exponent();
    e2a = x2.exponent();

    bool ce;
    int16_t tde;
    if (e1a > e2a) {
        ce = 0;
        tde = e1a - e2a;
    } else {
        ce = 1;
        tde = e2a - e1a;
    }

    int32_t de;
    if (tde > 31) {
        de = 31;
    } else {
        de = tde & 0x1F;
    }

    bool sel = ce;
    if (de == 0) {
        if (m1a > m2a) {
            sel = 0;
        } else {
            sel = 1;
        }
    }

    uint32_t ms;
    uint64_t mi;
    uint16_t es;
    uint16_t ei;
    bool ss;
    if (sel == 0) {
        ms = m1a;
        mi = m2a;
        es = e1a;
        ei = e2a;
        ss = x1.sign();
    } else {
        ms = m2a;
        mi = m1a;
        es = e2a;
        ei = e1a;
        ss = x2.sign();
    }

    uint64_t mie = mi << 31;
    uint64_t mia = mie >> de;

    bool tstck = 0;
    if ((mia & 0x1FFFFFFF) != 0) {
        tstck = 1;
    }

    uint32_t mye;
    if (x1.sign() == x2.sign()) {
        mye = (ms << 2) + (mia >> 29);
    } else {
        mye = (ms << 2) - (mia >> 29);
    }

    uint16_t esi = es + 1;

    uint32_t myd;
    uint16_t eyd;
    bool stck;
    
    eyd = (((mye >> 26) & 0x1)) ? ((esi == 0xFF) ? 0xFF : esi) : es;
    myd = (((mye >> 26) & 0x1)) ? ((esi == 0xFF) ? (1 << 25) : mye >> 1) : mye;
    stck = (((mye >> 26) & 0x1)) ? ((esi == 0xFF5) ? 0 : tstck || (mye & 0x1)) : tstck;

    uint16_t se;
    if (((myd >> 25) & 0x1) != 0) {
        se = 0;
    } else if (((myd >> 24) & 0x1) != 0) {
        se = 1;
    } else if (((myd >> 23) & 0x1) != 0) {
        se = 2;
    } else if (((myd >> 22) & 0x1) != 0) {
        se = 3;
    } else if (((myd >> 21) & 0x1) != 0) {
        se = 4;
    } else if (((myd >> 20) & 0x1) != 0) {
        se = 5;
    } else if (((myd >> 19) & 0x1) != 0) {
        se = 6;
    } else if (((myd >> 18) & 0x1) != 0) {
        se = 7;
    } else if (((myd >> 17) & 0x1) != 0) {
        se = 8;
    } else if (((myd >> 16) & 0x1) != 0) {
        se = 9;
    } else if (((myd >> 15) & 0x1) != 0) {
        se = 10;
    } else if (((myd >> 14) & 0x1) != 0) {
        se = 11;
    } else if (((myd >> 13) & 0x1) != 0) {
        se = 12;
    } else if (((myd >> 12) & 0x1) != 0) {
        se = 13;
    } else if (((myd >> 11) & 0x1) != 0) {
        se = 14;
    } else if (((myd >> 10) & 0x1) != 0) {
        se = 15;
    } else if (((myd >> 9) & 0x1) != 0) {
        se = 16;
    } else if (((myd >> 8) & 0x1) != 0) {
        se = 17;
    } else if (((myd >> 7) & 0x1) != 0) {
        se = 18;
    } else if (((myd >> 6) & 0x1) != 0) {
        se = 19;
    } else if (((myd >> 5) & 0x1) != 0) {
        se = 20;
    } else if (((myd >> 4) & 0x1) != 0) {
        se = 21;
    } else if (((myd >> 3) & 0x1) != 0) {
        se = 22;
    } else if (((myd >> 2) & 0x1) != 0) {
        se = 23;
    } else if (((myd >> 1) & 0x1) != 0) {
        se = 24;
    } else if ((myd & 0x1) != 0) {
        se = 25;
    } else {
        se = 26;
    }

    uint32_t myf_f, myf_s;
    myf_f = myd << se;
    myf_s = myd << ((eyd - 1) & 0x1F);

    int16_t eyf = eyd - se;
    uint16_t eyr;
    uint32_t myf;
    eyr = (eyf > 0) ? (eyf & 0xFF) : 0;
    myf = (eyf > 0) ? myd << se : myd << ((eyd & 0x1F) - 1);

    uint32_t myr;
    myr = (((myf & 0x7) == 0x6 && stck == 0) 
           || ((myf & 0x3) == 0x2 && x1.sign() == x2.sign() && stck == 1) 
           || ((myf & 0x3) == 0x3)) ? ((myf >>2) & 0x1FFFFFF) + 1 : ((myf >>2) & 0x1FFFFFF);  

    uint16_t eyri, ey;
    uint32_t my;
    eyri = eyr + 1;
    if (((myr >> 24) & 0x1) != 0) {
        ey = eyri;
        my = 0;
    } else if ((myr & 0xFFFFFF) == 0) {
        ey = 0;
        my = 0;
    } else {
        ey = eyr;
        my = myr & 0x7FFFFF;
    }

    bool sy;
    if (ey == 0 && my == 0) {
        sy = x1.sign() && x2.sign();
    } else {
        sy = ss;
    }

    // 結果を構成
    uint32_t result_raw;
    result_raw = (sy << 31) | ((ey & 0xFF) << 23) | (my & 0x7FFFFF);
    
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}

float fsub(float f1, float f2) {
    return fadd(f1, -f2);
}

float fmul(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    // 例外処理
    if (x1.exponent() == 0 || x2.exponent() == 0) return 0.0f;

    // 符号計算
    bool sign = x1.sign() ^ x2.sign();

    // 指数計算
    int32_t ey = x1.exponent() + x2.exponent() + 129;
    int32_t eyr = ey + 1;
    int32_t ee, mm;

    // 仮数計算
    uint32_t mh1 = (1 << 12) | ((x1.raw >> 11) & 0xFFF);
    uint32_t mh2 = (1 << 12) | ((x2.raw >> 11) & 0xFFF);
    uint32_t ml1 = x1.raw & 0x7FF;
    uint32_t ml2 = x2.raw & 0x7FF;

    uint32_t hh = mh1 * mh2;
    uint32_t hl = mh1 * ml2;
    uint32_t lh = ml1 * mh2;

    uint32_t m = hh + (hl >> 11) + (lh >> 11) + 2;

    if((x1.exponent() == 0) || (x2.exponent() == 0) || (((ey >> 8) & 0x1) == 0)){
        ee = 0;
    } else if (((m >> 25) & 0x1)) {
        ee = eyr & 0xFF;
    } else {
        ee = ey & 0xFF;
    }

    if((ee & 0xFF) == 0){
        mm = 0;
    } else if(((m >> 25) & 0x1)) {
        mm = (m >> 2) & 0x7FFFFF;
    } else {
        mm = (m >> 1) & 0x7FFFFF;
    }

    // 結果を構成
    uint32_t result_raw;
    result_raw = (sign << 31) | ((ee & 0xFF) << 23) | (mm & 0x7FFFFF);
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}

uint64_t lookup_inverse(uint16_t index) {
    if (index < 1024) {
        return inv_table[index];
    }
    return 0;
}

float fdiv(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    // 例外処理
    if (x1.exponent() == 0) return 0.0f;

    uint64_t table_entry = lookup_inverse(x2.mantissa() >> 13);
    uint32_t a = table_entry >> 32;
    uint32_t b = table_entry & 0xFFFFFFFF;
    float af = *reinterpret_cast<float*>(&a);
    float bf = *reinterpret_cast<float*>(&b);

    uint32_t rx = (0x7F << 23) | x2.mantissa();
    float rxf = *reinterpret_cast<float*>(&rx);

    float axf = fmul(af, rxf);
    float fxf = fsub(bf, axf);

    rx = (0x7F << 23) | x1.mantissa();
    rxf = *reinterpret_cast<float*>(&rx);

    float fyf = fmul(rxf, fxf);
    uint32_t fy = *reinterpret_cast<uint32_t*>(&fyf);

    // 符号計算
    bool sign = x1.sign() ^ x2.sign();

    // 指数計算
    int32_t e = 256 + x1.exponent() - x2.exponent() + (fy >> 23 & 0xFF);

    // 結果を構成
    uint32_t result_raw;
    if ((e & 0x100) == 0) {
        result_raw = (sign << 31) | (fy & 0x7FFFFF);
    } else {
        result_raw = (sign << 31) | ((e & 0xFF) << 23) | (fy & 0x7FFFFF);
    }
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}


uint32_t lookup_sqrt(uint16_t index) {
    if (index < 2048) {
        return sqrt_table[index];
    }
    return 0;
}

float fsqrts(float f1) {
    FP32 x = { *reinterpret_cast<uint32_t*>(&f1) };

    // 例外処理
    if (x.exponent() == 0) return 0.0f;

    uint16_t idx;
    if ((x.exponent() & 0x1) == 0) {
        idx = 0x200 | (x.mantissa() >> 14);
    } else {
        idx = x.mantissa() >> 14;
    }

    // uint64_t table_entry = lookup_sqrt(idx);
    uint32_t a = lookup_sqrt(2 * idx);
    uint32_t b = lookup_sqrt(2 * idx + 1);
    float af = *reinterpret_cast<float*>(&a);
    float bf = *reinterpret_cast<float*>(&b);

    uint32_t rx;
    if ((x.exponent() & 1) == 0) {
        rx = (0x80 << 23) | x.mantissa();
    } else {
        rx = (0x7F << 23) | x.mantissa();
    }
    float rxf = *reinterpret_cast<float*>(&rx);

    float axf = fmul(af, rxf);
    float fxf = fadd(bf, axf);

    uint32_t fx = *reinterpret_cast<uint32_t*>(&fxf);

    // 指数計算
    int32_t e = x.exponent();
    if ((e & 0x80) != 0) {
        e = e - 127;
        e = 127 + (e >> 1);
    } else {
        e = 128 - e;
        e = 127 - (e >> 1);
    }

    e = e + ((fx >> 23) & 0xFF) + 129;

    // 結果を構成
    uint32_t result_raw;
    if ((e & 0x100) == 0) {
        result_raw = fx & 0x7FFFFF;
    } else {
        result_raw = ((e & 0xFF) << 23) | (fx & 0x7FFFFF);
    }
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}
float fcvtsw(int32_t x) {
    uint32_t s = (x < 0) ? 1 : 0; // 符号ビット

    // 絶対値を計算
    uint32_t x_uint = (x >> 31) ? (~static_cast<uint32_t>(x) + 1) : static_cast<uint32_t>(x);

    // 指数部と正規化
    uint8_t msb = 0; //msb
    msb = ((x_uint >> 31) & 0x1) ? 1 : 
        ((x_uint >> 30) & 0x1) ? 2 : 
        ((x_uint >> 29) & 0x1) ? 3 :
        ((x_uint >> 28) & 0x1) ? 4 :
        ((x_uint >> 27) & 0x1) ? 5 :
        ((x_uint >> 26) & 0x1) ? 6 :
        ((x_uint >> 25) & 0x1) ? 7 :
        ((x_uint >> 24) & 0x1) ? 8 :
        ((x_uint >> 23) & 0x1) ? 9 :
        ((x_uint >> 22) & 0x1) ? 10 :
        ((x_uint >> 21) & 0x1) ? 11 :
        ((x_uint >> 20) & 0x1) ? 12 :
        ((x_uint >> 19) & 0x1) ? 13 :
        ((x_uint >> 18) & 0x1) ? 14 :
        ((x_uint >> 17) & 0x1) ? 15 :
        ((x_uint >> 16) & 0x1) ? 16 :
        ((x_uint >> 15) & 0x1) ? 17 :
        ((x_uint >> 14) & 0x1) ? 18 :
        ((x_uint >> 13) & 0x1) ? 19 :
        ((x_uint >> 12) & 0x1) ? 20 :
        ((x_uint >> 11) & 0x1) ? 21 :
        ((x_uint >> 10) & 0x1) ? 22 :
        ((x_uint >> 9) & 0x1) ? 23 :
        ((x_uint >> 8) & 0x1) ? 24 :
        ((x_uint >> 7) & 0x1) ? 25 :
        ((x_uint >> 6) & 0x1) ? 26 :
        ((x_uint >> 5) & 0x1) ? 27 :
        ((x_uint >> 4) & 0x1) ? 28 :
        ((x_uint >> 3) & 0x1) ? 29 :
        ((x_uint >> 2) & 0x1) ? 30 :
        ((x_uint >> 1) & 0x1) ? 31 :
        (x_uint & 0x1) ? 32 : 0;
    
    uint32_t x_shift;
    x_shift = x_uint << msb;

    uint32_t mf;
    mf = (x_shift >> 9) & 0x7FFFFF;

    uint32_t mf_i;
    mf_i = mf + ((x_shift >> 8) & 0x1);

    uint8_t ef;
    ef = (msb == 0) ? 0 :
                 (((mf & 0xFF) == 0xFF) && ((x_shift >> 8) & 0x1)) ? 160 - msb : 159 - msb;
    
    // 結果を構成
    uint32_t result_raw = (s << 31) | (ef << 23) | (mf_i & 0x7FFFFF);
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}

int32_t fcvtws(float f1) {

    FP32 x = { *reinterpret_cast<uint32_t*>(&f1) };

    uint16_t e = x.exponent();
    uint32_t m = x.mantissa();

    uint32_t z = 0;
    if (e > 157) {
        z = 0x7FFFFFFF;
    } else if (e == 157) {
        z = (1 << 30) | (m << 7);
    } else if (e >= 126) {
        uint32_t shift = 157 - e;
        uint32_t tmp = (1 << 30) | (m << 7);
        uint32_t round_bit = (tmp >> (shift - 1)) & 0x1;
        z = (tmp >> shift) + round_bit;
    }

    int32_t result = x.sign() ? -(int32_t)z : (int32_t)z;
    return result;
}

bool feq(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    bool f = 0;
    if (x1.raw == x2.raw) {
        f = 1;
    }
    return f;
}

bool fle(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    bool s1 = x1.sign();
    bool s2 = x2.sign();
    uint16_t e1 = x1.exponent();
    uint16_t e2 = x2.exponent();
    uint32_t m1 = x1.mantissa();
    uint32_t m2 = x2.mantissa();

    bool f = 0;
    if (s1 == 0 && s2 == 0) {
        if (e1 < e2) {
            f = 1;
        } else if (e1 == e2) {
            if (m1 <= m2) {
                f = 1;
            }
        }
    } else if (s1 == 1 && s2 == 0) {
        f = 1;
    } else if (s1 == 1 && s2 == 1) {
        if (e1 > e2) {
            f = 1;
        } else if (e1 == e2) {
            if (m1 >= m2) {
                f = 1;
            }
        }
    }
    if (e1 == 0 && e2 == 0) {
        f = 1;
    }
    return f;
}

bool flt(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    bool s1 = x1.sign();
    bool s2 = x2.sign();
    uint16_t e1 = x1.exponent();
    uint16_t e2 = x2.exponent();
    uint32_t m1 = x1.mantissa();
    uint32_t m2 = x2.mantissa();

    bool f = 0;
    //両方+-0.0の時0をreturn
    if(((x1.raw & 0x7FFFFFFF) == 0) && ((x2.raw & 0x7FFFFFFF) == 0)) {
        return 0;
    }
    if (s1 == 0 && s2 == 0) {
        if (e1 < e2) {
            f = 1;
        } else if (e1 == e2) {
            if (m1 < m2) {
                f = 1;
            }
        }
    } else if (s1 == 1 && s2 == 0) {//f1が負、x2が正
        f = 1; //true
    } else if (s1 == 1 && s2 == 1) {//両方負
        if (e1 > e2) {
            f = 1;
        } else if (e1 == e2) {
            if (m1 > m2) {
                f = 1;
            }
        }
    }
    return f;
}

float fsgnj(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    uint32_t result_raw = (x2.sign() << 31) | ((x1.exponent() & 0xFF) << 23) | (x1.mantissa() & 0x7FFFFF);
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}

float fsgnjn(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    uint32_t result_raw = (!x2.sign() << 31) | ((x1.exponent() & 0xFF) << 23) | (x1.mantissa() & 0x7FFFFF);
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}

float fsgnjx(float f1, float f2) {
    FP32 x1 = { *reinterpret_cast<uint32_t*>(&f1) };
    FP32 x2 = { *reinterpret_cast<uint32_t*>(&f2) };

    uint32_t result_raw = ((x1.sign() ^ x2.sign()) << 31) | ((x1.exponent() & 0xFF) << 23) | (x1.mantissa() & 0x7FFFFF);
    float result = *reinterpret_cast<float*>(&result_raw);
    return result;
}