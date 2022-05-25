#ifndef SRC_S21_MATH_H_
#define SRC_S21_MATH_H_

#define INF 1.0/0.0
#define NAN 0.0/0.0

#define S21_PI 3.1415926535897931159
#define S21_E 2.71828182845904523

void set_precision(long double digits);

int s21_abs(int x);
int s21_abs(int x);
long double s21_fabs(long double x);

long double s21_fmod(long double x, long double y);
long double s21_floor(long double x);
long double s21_ceil(long double x);

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
