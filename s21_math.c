#include "s21_math.h"
#include <stdint.h>

#define OCTALC sizeof(long double)/sizeof(int64_t)
#define SIGNBIT 0x8000
#define EXPBITS 0x7FFE
#define EXPPOS 112
#define MANBITC 63

long double pade_c[2][8] = { 
    {0.0000000578125l, 0.0000032375l, 0.0000874125l, \
     0.001456876l, 0.016025641025l, 0.1153846l, 0.5l, 1},
    {-0.0000000578125l, 0.0000032375l, -0.0000874125l, \
     0.001456876l, -0.016025641025l, 0.1153846l, -0.5l, 1},
};

int isfinite(long double x) {
    return x == x || x < INF || x > -INF;
}
int s21_abs(int x){
    if (x < 0)
        return -x;
    else
        return x;
}
long double s21_fabs(long double x) {
    if (x < 0)
        return -x;
    else
        return x;
}

long double s21_fmod(long double x, long double y) {
    long double i = s21_floor(x/y);
    return x - i*y;
}
long double s21_floor(long double x) {
    union flt {
        long double flt;
        int64_t byte8[OCTALC];
    } d;
    d.flt = x;
    unsigned short sign, exp, ofs; 
    short hbyte = OCTALC - 1;
    sign = d.byte8[hbyte] & SIGNBIT;
    exp = (d.byte8[hbyte] & EXPBITS) - 16383;
    ofs = MANBITC - exp;
    if (exp < 0)
        return -(long double)sign;
    for (int l = 0; l < hbyte + 1 && ofs > 0; l++)
        for (int b = 0; b < 64 && (b + l*64) < ofs; b++)
            d.byte8[l] &= ~((int64_t)1 << b);
    return d.flt;
}
long double s21_ceil(long double x) {
    return s21_floor(x + 1);
}

long double s21_sin(long double x);
long double s21_cos(long double x);
long double s21_tan(long double x);

long double s21_asin(long double x);
long double s21_acos(long double x);
long double s21_atan(long double x);

long double s21_ipow(long double base, long double exp) {
    long double  res = 1;
    for (int i  = 0; i < exp; i++)
        res *= base;
    return res;
}
long double s21_pow(long double base, long double exp) {
    
}
long double s21_exp(long double x) {
    long double ef = 0, ei, intg = s21_floor(x), frac = x - intg, P = 0, Q = 0;
    ei = s21_ipow(S21_E, intg);
    for (int i = 1; i < 20; i++);
    return ei*ef;
}
long double s21_sqrt(long double x) {
    long double xn = x/2;
    for (int n = 0; s21_fabs(x - xn*xn) > 1e-18 && isfinite(x); n++)
        xn = 0.5l*(xn + x/xn);
    return xn;
}
long double s21_log(long double x);
