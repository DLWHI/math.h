#include "s21_math.h"
#include <stdint.h>

#define OCTALC sizeof(long double)/sizeof(int64_t)
#define EXPBITS 0x7FFF
#define MANBITC 63

long double c_sin[] = {
    0,
    -0.42476506015273822865,
    0,
    -0.058224392078514572072,
    0,
    0.74564931593694056499,
    0,
    -0.31504226022478564438,
    0,
    0.058247768279542445596,
    0,
    -0.0062956504847546091678,
    0,
    0.00045277117470981463257,
    0,
    -2.3376886594574810396e-05,
    0,
    9.1161719852743175551e-07,
    0,
    -2.7852910787037387064e-08,
    0,
    6.8556995117086238587e-10,
    0,
    -1.3895858880252567668e-11,
    0,
    2.3609810458726357887e-13,
    0,
    -3.4124466240223290358e-15,
    0,
    4.248069303240149625e-17,
    0,
    -4.6033425716743938346e-19,
    0,
    4.3822134883976752157e-21,
    0,
    -3.6943087026160027476e-23,
    0,
    2.7775492482464429791e-25,
    0,
    -1.8741618426444227021e-27
};
long double c_exp[] = {
    1.2660658777520083355947258518803266724717,
    1.1303182079849700544,
    0.27149533953407656238,
    0.044336849848663804959,
    0.0054742404420937326501,
    0.00054292631191394375043,
    4.4977322954295146654e-05,
    3.1984364624019905058e-06,
    1.992124806672795726e-07,
    1.1036771725517344327e-08,
    5.5058960796737472502e-10,
    2.4979566169849825227e-11,
    1.0391522306785700505e-12,
    3.9912633564144015131e-14,
    1.4237580108256571487e-15,
    4.7409261025614961712e-17,
    1.4801800572082975004e-18,
    4.3499194949441698455e-20,
    1.2074289272797528892e-21,
    3.1753567370594449604e-23,
};

long double pow2i(long double exp) {
    union flt {
        long double flt;
        int64_t byte8[OCTALC];
    } d;
    d.flt = 1.0l;
    short hbyte = OCTALC - 1;
    d.byte8[hbyte] = (d.byte8[hbyte] & EXPBITS) + exp;
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
long double summate_chebyshev(long double x, long double* c, int count) {
    long double bn, bn_1 = 0, bn_2 = 0;
    for (int i = count - 1; i > 0; i--) {
         bn = c[i] + 2.0*x*bn_1 - bn_2;
         bn_2 = bn_1;
         bn_1 = bn;
    }
    bn = 2.0l*c[0] + 2.0*x*bn_1 - bn_2;
    return (bn - bn_2)/2.0l;
}

int isfinite(long double x) {
    return x == x && x < INF && x > -INF;
}
int isinf(long double x) {
    return x == INF || x == -INF;
}
int isnan(long double x) {
    return !(x == x);
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

long double s21_trunc(long double x) {
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
    return s21_trunc(x + 0.5l);
}
long double s21_fmod(long double x, long double y) {
    long double i = s21_trunc(x/y);
    if (isinf(y) && isfinite(x))
        return x;
    return x - i*y;
}

long double s21_sin(long double x) {
    if (!isfinite(x))
        return NAN;
    x =  s21_fmod(x, S21_2PI);
    return summate_chebyshev(x/S21_2PI, c_sin, 40);
}
long double s21_cos(long double x) {
    if (!isfinite(x))
        return NAN;
    return s21_sin(S21_PId2 - x);
}
long double s21_tan(long double x) {
    if (!isfinite(x))
        return NAN;
    int n = 63;
    long double u[2] = {1, 0};
    long double ysum = 0, s;  // K = 0.60725293500888127872l
    long double y[] = {
        0.78539816339744830963,
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
    for (int i = 0; i < n; ysum += s*y[i], i++) {
        s = (x > ysum) - (x < ysum);
        long double v[2];
        v[0] = u[0] - s*pow2i(-i)*u[1];
        v[1] = s*pow2i(-i)*u[0] + u[1];
        u[0] = v[0];
        u[1] = v[1];
    }
    return u[1]/u[0];
}

long double cordic_rotation_to0(long double sin, long double cos) {
    int n = 75, i = 0;
    long double u[2] = {cos, sin};
    long double alpha = 0, s;
    long double y[] = {
        0.78539816339744830963,
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
        2.168404344971008868e-19,
        1.084202172485504434e-19,
        5.42101086242752217e-20,
        2.710505431213761085e-20,
        1.3552527156068805425e-20,
        6.7762635780344027125e-21,
        3.3881317890172013563e-21,
        1.6940658945086006781e-21,
        8.4703294725430033907e-22,
        4.2351647362715016953e-22,
        2.1175823681357508477e-22,
        1.0587911840678754238e-22,
        5.2939559203393771192e-23
    };
    for (; i < n && y[i] > __LDBL_EPSILON__; alpha += s*y[i], i++) {
        s = (0 > u[1]) - (0 < u[1]);
        long double v[2];
        v[0] = u[0] - s*pow2i(-i)*u[1];
        v[1] = s*pow2i(-i)*u[0] + u[1];
        u[0] = v[0];
        u[1] = v[1];
    }
    return -alpha;
}
long double s21_asin(long double x) {
    if (-1 > x || x > 1 || !isfinite(x))
        return NAN;
    return cordic_rotation_to0(x, s21_sqrt(1 - x*x));
}
long double s21_acos(long double x) {
    if (-1 > x || x > 1 || !isfinite(x))
        return NAN;
    return cordic_rotation_to0(s21_sqrt(1 - x*x), s21_fabs(x)) + S21_PId2*(x < 0) + S21_PId2*(x == -1);
}
long double s21_atan(long double x) {
    if (!isfinite(x))
        return x;
    return cordic_rotation_to0(x/s21_sqrt(1 - x*x), 1.0l/s21_sqrt(1 - x*x));
}

long double s21_pow(long double base, long double exp) {
    if (!(exp - s21_floor(exp)) && base == 2.0l)
        return pow2i(exp);
    if (exp - s21_floor(exp) && exp > 0)
        return s21_exp(exp*s21_log(base));
    else
        return powi(base, exp); 
}
long double s21_exp(long double x) {
    long double i = s21_floor(x/S21_LN2 + 0.5l), f = x - i*S21_LN2;
    long double ei, ef = 0;
    ei = pow2i(i);
    ef = summate_chebyshev(f, c_exp, 17);
    return ei*ef;
}
long double s21_sqrt(long double x) {
    if (x < 0)
        return NAN;
    long double xn = x/2;
    while(s21_fabs(x - xn*xn) > 1e-18 && isfinite(x))
        xn = 0.5l*(xn + x/xn);
    return xn;
}
// Modified Newthon-Raphson solving e^y/2 - ce^-y/2 = 0 where c is x = c*10^n
// time complexity bound to O(exp(x) + log10(x)), precision ~17 digits
long double s21_log(long double x) {
    if (x < 0)
        return NAN;
    long double exp = 0, y, ln = 0;
    if (x >= 10.0)
        for (; x >= 10.0; x /= 10, exp++) { }
    else if (x < 0.1)
        for (; x < 0.1; x *= 10, exp--) { }
    while(s21_fabs(s21_exp(ln) - x) > 1e-18 && isfinite(x))
        ln += 2.0*(x - s21_exp(ln))/(x + s21_exp(ln));
    return ln + exp*S21_LN10;
}
// Newthon-Raphson solving e^y/2 - xe^-y/2 = 0
// time complexity bound to O(exp(x)) slows at big x, precision ~17 digits 
// long double s21_log(long double x) {
//     long double ln = 0;
//     while(s21_fabs(s21_exp(ln) - x) > 1e-14)
//         ln += 2.0*(x - s21_exp(ln))/(x + s21_exp(ln));
//     return ln;
// }
