#ifndef SRC_S21_MATH_H_
#define SRC_S21_MATH_H_

#ifndef INF
#define INF 1.0/0.0
#endif
#ifndef NAN
#define NAN 0.0/0.0
#endif

#define S21_PI 3.1415926535897931159l
#define S21_PI_2 1.5707963267948966l
#define S21_2PI 6.283185307179586l
#define S21_E 2.71828182845904523l
#define S21_LN2 0.69314718055994530l
#define S21_LN10 2.3025850929940456l

int s21_abs(int x);
long double s21_fabs(long double x);

long double s21_floor(long double x);
long double s21_ceil(long double x);
long double s21_trunc(long double x);
long double s21_round(long double x);
long double s21_fmod(long double x, long double y);

long double s21_sin(long double x);
long double s21_cos(long double x);
long double s21_tan(long double x);

long double s21_acos(long double x);
long double s21_asin(long double x);
long double s21_atan(long double x);

long double s21_pow(long double base, long double exp);
long double s21_exp(long double x);
long double s21_sqrt(long double x);
long double s21_log(long double x);

#endif  // SRC_S21_MATH_H_
