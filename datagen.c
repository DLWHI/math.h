#include <stdio.h>
#include <stdlib.h>
#include "s21_math.h"
#include <stdint.h>
#include <math.h>

void print_bytes(void* ptr, int size) {
    for (int i = 0; i < size; i++)
        printf("%02X ", *((unsigned char*)ptr + i));
}
long double fact(int n) {
    long double f = 1;
    for (int i = 0; i < n; i++)
        f *= i + 1;
    return f;
}
long double besselI(long double n) {
    long double I = 0;
    int order = 5000;
    for (int m = 0; m < order; m++)
        I += s21_pow(2.0l, -2*m - n)/(fact(m)*fact(m + n));
    return I;
}
long double besselJ(long double n) {
    long double J = 0;
    int order = 50;
    for (int m = 0; m < order; m++)
        J += s21_pow(-1, m)*s21_pow(S21_PI, 2*m + n)/(fact(m)*fact(m + n));
    return J;
}
void gen_besselI(const int count) {
    printf("%.40Lf,\n", besselI(0));
    for (int i = 1; i < count; i++)
        printf("%.20Lg,\n", 2.0l*besselI(i));
}
void gen_besselJ(const int count) {
    printf("%.40Lf,\n", besselJ(0));
    for (int i = 1; i < 2*count; i++)
        printf("%.20Lg,\n", 2.0*besselJ(i));
}
void gen_atan(const int count) {
    long double theta[count];
    for (int i = 0; i < count; i++) {
        theta[i] = s21_pow(2, -i);
        printf("        %.20Lg,\n", atanl(theta[i]));
    }
    long double K = 1;
    for (int i = 0; i < count; i++)
        K *= 1/s21_sqrt(1 + pow(2, -2*i));
    printf("%.20Lg,\n", K);
}

int main() {
    gen_atan(75);
    return 0;
}