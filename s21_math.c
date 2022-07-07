#include "s21_math.h"
#include <stdint.h>

#define OCTALC sizeof(long double)/sizeof(int64_t)
#define EXPBITS 0x7FFF
#define MANBITC 63

long double tangent_angles[] = {
        S21_PI_2/2.0l,
        0.4636476090008061162,
        0.24497866312686415417,
        0.12435499454676143503,
        0.062418809995957348475,
        0.031239833430268276253,
        0.015623728620476830803,
        0.0078123410601011112965,
        0.0039062301319669718277,
        0.0019531225164788186852,
        0.00097656218955931943043,
        0.00048828121119489827547,
        0.00024414062014936176402,
        0.00012207031189367020424,
        6.1035156174208775022e-05,
        3.0517578115526096862e-05,
        1.5258789061315762107e-05,
        7.6293945311019702635e-06,
        3.814697265606496283e-06,
        1.9073486328101870354e-06,
        9.5367431640596087944e-07,
        4.7683715820308885994e-07,
        2.384185791015579825e-07,
        1.1920928955078068531e-07,
        5.9604644775390554415e-08,
        2.9802322387695303677e-08,
        1.4901161193847655147e-08,
        7.4505805969238279873e-09,
        3.7252902984619140453e-09,
        1.8626451492309570291e-09,
        9.3132257461547851537e-10,
        4.6566128730773925779e-10,
        2.3283064365386962891e-10,
        1.1641532182693481445e-10,
        5.8207660913467407227e-11,
        2.9103830456733703613e-11,
        1.4551915228366851807e-11,
        7.2759576141834259033e-12,
        3.6379788070917129517e-12,
        1.8189894035458564758e-12,
        9.0949470177292823792e-13,
        4.5474735088646411896e-13,
        2.2737367544323205948e-13,
        1.1368683772161602974e-13,
        5.684341886080801487e-14,
        2.8421709430404007435e-14,
        1.4210854715202003717e-14,
        7.1054273576010018587e-15,
        3.5527136788005009294e-15,
        1.7763568394002504647e-15,
        8.8817841970012523234e-16,
        4.4408920985006261617e-16,
        2.2204460492503130808e-16,
        1.1102230246251565404e-16,
        5.5511151231257827021e-17,
        2.7755575615628913511e-17,
        1.3877787807814456755e-17,
        6.9388939039072283776e-18,
        3.4694469519536141888e-18,
        1.7347234759768070944e-18,
        8.6736173798840354721e-19,
        4.336808689942017736e-19,
        2.168404344971008868e-19
};

// Auxilary functions
long double pow2i(long double exp) {
    union flt {
        long double flt;
        int64_t byte8[OCTALC];
    } d;
    d.flt = 1.0l;
    d.byte8[OCTALC - 1] = (d.byte8[OCTALC - 1] & EXPBITS) + exp;
    return d.flt;
}
long double powi(long double base, long double exp) {
    long double res = 1;
    while (exp > 0) {
        if (s21_fmod(exp, 2)) {
            res *= base;
            exp -= 1;
        }
        else {
            base *= base;
            exp /= 2;
        }
    }
    return res;
}

int s21_isfinite(long double x) {
    return x == x && x < INF && x > -INF;
}
int s21_isinf(long double x) {
    return x == INF || x == -INF;
}
int s21_isnan(long double x) {
    return !(x == x);
}
int s21_iswhole(long double x) {
    if (!s21_isfinite(x))
        return 0;
    else if (!x)
        return 1;
    union flt {
        long double flt;
        int64_t byte8[OCTALC];
    } d;
    d.flt = x;
    short exp, ofs; 
    short hbyte = OCTALC - 1;
    exp = (d.byte8[hbyte] & EXPBITS) - 16383;
    ofs = MANBITC - exp;
    if (exp < 0)
        return 0;
    for (int l = 0; l < hbyte + 1 && ofs > 0; l++)
        for (int b = 0; b < 64 && (b + l*64) < ofs; b++)
            if (d.byte8[l] & ((int64_t)1 << b))
                return 0;
    return 1;
}

// Arithmetic funtions
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

long double s21_trunc(long double x) {
    union flt {
        long double flt;
        int64_t byte8[OCTALC];
    } d;
    d.flt = x;
    short exp, ofs;
    exp = (d.byte8[OCTALC - 1] & EXPBITS) - 16383;
    ofs = MANBITC - exp;
    if (exp < 0)
        return 0;
    for (int l = 0; l < OCTALC && ofs > 0; l++)
        for (int b = 0; b < 64 && (b + l*64) < ofs; b++)
            d.byte8[l] &= ~((int64_t)1 << b);
    return d.flt;
}
long double s21_floor(long double x) {
    return s21_trunc(x) - (x < 0);
}
long double s21_ceil(long double x) {
    return s21_floor(x + 1);
}
long double s21_round(long double x) {
    if (x > 0)
        return s21_trunc(x + 0.5l);
    else
        return s21_trunc(x - 0.5l);
}
long double s21_fmod(long double x, long double y) {
    long double i = s21_trunc(x/y);
    if (s21_isinf(y) && s21_isfinite(x))
        return x;
    return x - i*y;
}


// Trigonometric functions
long double chebyshev_sin(long double x) {
    const long double c_sin[] = {
        -0.42476506015273822865,
        -0.058224392078514572072,
        0.74564931593694056499,
        -0.31504226022478564438,
        0.058247768279542445596,
        -0.0062956504847546091678,
        0.00045277117470981463257,
        -2.3376886594574810396e-05,
        9.1161719852743175551e-07,
        -2.7852910787037387064e-08,
        6.8556995117086238587e-10,
        -1.3895858880252567668e-11,
        2.3609810458726357887e-13,
        -3.4124466240223290358e-15,
        4.248069303240149625e-17,
        -4.6033425716743938346e-19,
        4.3822134883976752157e-21,
        -3.6943087026160027476e-23,
        2.7775492482464429791e-25,
        -1.8741618426444227021e-27
    };
    long double bn, bn_1 = 0, bn_2 = 0;
    for (int i = 39; i > 0; i--) {
        long double t = i%2 ? c_sin[i/2] : 0;
        bn = t + 2.0*x*bn_1 - bn_2;
        bn_2 = bn_1;
        bn_1 = bn;
    }
    bn = 2.0*x*bn_1 - bn_2;
    return (bn - bn_2)/2.0l;
}
long double cordic_rotation_to0(long double sin, long double cos) {
    int n = 63;
    long double u[2] = {cos, sin};
    long double alpha = 0, s;
    for (int i = 0; i < n; alpha += s*tangent_angles[i], i++) {
        s = (0 > u[1]) - (0 < u[1]);
        long double v[2];
        v[0] = u[0] - s*pow2i(-i)*u[1];
        v[1] = s*pow2i(-i)*u[0] + u[1];
        u[0] = v[0];
        u[1] = v[1];
    }
    return -alpha;
}
long double cordic_rotation_tox(long double x) {
    int n = 63;
    long double u[2] = {1, 0};
    long double ysum = 0, s;  // K = 0.60725293500888127872l
    for (int i = 0; i < n; ysum += s*tangent_angles[i], i++) {
        s = (x > ysum) - (x < ysum);
        long double v[2];
        v[0] = u[0] - s*pow2i(-i)*u[1];
        v[1] = s*pow2i(-i)*u[0] + u[1];
        u[0] = v[0];
        u[1] = v[1];
    }
    return u[1]/u[0];
}
long double s21_sin(long double x) {
    if (!s21_isfinite(x))
        return NAN;
    x =  s21_fmod(x, S21_2PI);
    return chebyshev_sin(x/S21_2PI);
}
long double s21_cos(long double x) {
    if (!s21_isfinite(x))
        return NAN;
    return s21_sin(S21_PI_2 - x);
}
long double s21_tan(long double x) {
    if (!s21_isfinite(x))
        return NAN;
    return cordic_rotation_tox(x);
}

long double s21_asin(long double x) {
    if (-1 > x || x > 1 || !s21_isfinite(x))
        return NAN;
    return cordic_rotation_to0(x, s21_sqrt(1 - x*x));
}
long double s21_acos(long double x) {
    if (-1 > x || x > 1 || !s21_isfinite(x))
        return NAN;
    else if (x < 0)
        return S21_PI - cordic_rotation_to0(s21_sqrt(1 - x*x), -x);
    return cordic_rotation_to0(s21_sqrt(1 - x*x), x);
}
long double s21_atan(long double x) {
    if (!s21_isfinite(x))
        return NAN;
    return cordic_rotation_to0(x/s21_sqrt(1 + x*x), 1.0l/s21_sqrt(1 + x*x));
}


// Power functions
long double pade_exp(long double x) {
    const long double P[] = {
        1.0l,
        0.5l,
        19.0l/156.0l,
        1.0l/52.0l,
        17.0l/7696.0l,
        17.0l/86580.0l,
        17.0l/1212120.0l,
        1.0l/1212120.0l,
        1.0l/24615360.0l,
        1.0l/590768640.0l,
        1.0l/16648934400.0l,
        1.0l/549414835200.0l,
        1.0l/21244040294400.0l,
        1.0l/966603833395200.0l,
        1.0l/52196607003340800.0l,
        1.0l/3392779455217152000.0l,
        1.0l/271422356417372160000.0l,
        1.0l/27685080354571960320000.0l,
        1.0l/3820541088930930524160000.0l,
        1.0l/798493087586564479549440000.0l,
        1.0l/335367096786357081410764800000.0l
    };
    long double i = s21_floor(x/S21_LN2 + 0.5l), f = x - i*S21_LN2;
    long double ei, ef, Psum = 0, Qsum = 0;
    ei = pow2i(i);
    long double powx = powi(f, 20), sign = 1;
    for (int j = 20; j >= 0; j--) {
        Psum += powx*P[j];
        Qsum += powx*P[j]*sign;
        powx /= f;
        sign *= -1;
    }
    ef = Psum/Qsum;
    return ei*ef;
}
long double newthon_log(long double x) {
    // Modified Newthon-Raphson solving e^y/2 - ce^-y/2 = 0 where c is x = c*10^n
    long double exp = 0, ln = 0;
    if (x >= 10.0)
        for (; x >= 10.0; x /= 10, exp++) { }
    else if (x < 0.1)
        for (; x < 0.1; x *= 10, exp--) { }
    while(s21_fabs(s21_exp(ln) - x) > 1e-18)
        ln += 2.0*(x - s21_exp(ln))/(x + s21_exp(ln));
    return ln + exp*S21_LN10;
}

long double s21_pow(long double base, long double exp) {
    if (base == 1 && !s21_isinf(exp))
        return 1;
    else if (s21_iswhole(exp) && base == 2.0l)
        return pow2i(exp);
    else if (s21_iswhole(exp) && exp >= 0)
        return powi(base, exp);
    else if (s21_iswhole(exp) && exp < 0)
        return 1/powi(base, -exp);
    else if (base >= 0)
        return s21_exp(exp*s21_log(base));
    return NAN;
}
long double s21_exp(long double x) {
    if (x == INF)
        return INF;
    else if (x == -INF)
        return 0;
    else if (x == NAN)
        return NAN;
    else if (x == 0)
        return 1.0;
    else
        return pade_exp(x);
}
long double s21_sqrt(long double x) {
    if (x < 0)
        return NAN;
    long double xn = x/2;
    while(s21_fabs(x - xn*xn) > 1e-18 && s21_isfinite(x))
        xn = 0.5l*(xn + x/xn);
    return xn;
}
long double s21_log(long double x) {
    if (x < 0 || s21_isnan(x))
        return NAN;
    else if (x == INF)
        return INF;
    else if (x == 0)
        return -INF;
    else
        return newthon_log(x);
}
