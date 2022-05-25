#include <stdio.h>
#include "s21_math.h"
#include <math.h>

int main() {
    long double e21 = s21_exp(2.5);
    long double e = exp(2.5);
    printf("%Lf\n%Lf\n%Lg", e, e21, e21 - e);
    return 0;
}