#include <stdio.h>
#include <stdlib.h>
#include "s21_math.h"

long double** multiply(long double *a, long double *b, int m, int n) {
    long double **c = malloc(sizeof(long double*)*m + sizeof(long double)*n*m);
    for (int i = 0; i < m; i++)
        *(c + i) = (long double*)c + m*sizeof(long double*) + m*n*sizeof(long double)*i;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            c[i][j] = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j <= i; j++)
            c[i][j] = a[i - j]*b[j];
    }
    return c;
}
long double fact(int n) {
    long double f = 1;
    for (int i = 0; i < n; i++)
        f *= i + 1;
    return f;
}
void print(long double **a, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%Lf ", a[i][j]);
        printf("\n");
    }
}
long double* solve(long double** hsle, int m, int n) {
    long double *sol, *oldsol;
    if (m < n)
        return NULL;
    sol = malloc(sizeof(double)*n);
    oldsol = malloc(sizeof(double)*n);
    for (int i = 0; i < n; i++)
        oldsol[i] = 0.5l;
    for (int k = 0; k < 1000; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                if (i != j)
                    sol[i] -= hsle[i][j]/hsle[i][i]*oldsol[j];
        }
        for (int j = 0; j < n; j++)
            oldsol[j] += sol[j];
    }
    return sol;
}
long double* complete(long double** cefs, long double* q, int n, int m) {
    long double* p = malloc(sizeof(long double)*m);
    for (int i = 0; i < m; i++) {
        p[i] = 0;
        for (int j = 0; j < n; j++)
            p[i] +=q[j]*cefs[i][j];
    }
    return p;
}

int main() {
    const int m = 6, n = 6;
    long double ones[n], T[11], **cmtx, *Q, *P;
    for (int i = 0; i < n+m-1; i++)
        T[i] = 1.0l/fact(i);
    for (int i = 0; i < n; i++)
        ones[i] = 1;
    cmtx = multiply(T, ones, n+m-1, n);
    print(cmtx, m+n-1, n);
    Q = solve(&(cmtx[n - 1]), n, n);
    P = complete(cmtx, Q, n, m);
    printf("\n");
    print(&Q, 1, n);
    printf("\n");
    print(&P, 1, m);
    return 0;
}