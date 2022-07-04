#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DEGREE 5

void print_extended(long double** system, long double* added) {
    for (int i = 0; i < DEGREE; i++) {
        for (int j = 0; j < DEGREE; j++)
            printf("%Lg ", system[i][j]);
        printf("%Lg\n", added[i]);
    }
}
void print_matrix(long double** matrix) {
    for (int i = 0; i < DEGREE; i++) {
        for (int j = 0; j < DEGREE; j++)
            printf("%.20Lg ", matrix[i][j]);
        printf("\n");
    }
}
void print_vector(long double* vector) {
    for (int j = 0; j < DEGREE; j++)
        printf("%.20Lg,\n", -vector[j]);
}


void system_init(long double** coefs, long double* free) {
    for (int i = 0; i < DEGREE; i++)
        for (int j = 0; j < DEGREE; j++)
            coefs[i][j] = 1.0l/tgammal(i + DEGREE - j + 1);
    for (int i = 0; i < DEGREE; i++)
        free[i] = 1.0l/tgammal(i + DEGREE + 2);
}

long double* multiply(long double** matrix, long double* vector) {
    long double* result = malloc(sizeof(long double)*DEGREE);
    for (int i = 0; i < DEGREE; i++) {
        result[i] = 0;
        for (int j = 0; j < DEGREE; j++)
            result[i] += matrix[i][j] * vector[j];
    }
    return result;
}
long double** getIdentityMatrix(int n) {
    long double** I = malloc(sizeof(long double*)*n);
    for (int i = 0; i < n; i++)
        I[i] = malloc(sizeof(long double)*n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == j)
                I[i][j] = 1;
            else
                I[i][j] = 0;
    return I;
}
long double** invert(long double** matrix)
{
    long double** I = getIdentityMatrix(DEGREE);
    for (int i = 0; i < DEGREE; i++) 
    {
        for (int j = 0; j < DEGREE; j++)
        {
            if (i == j)
                continue;
            long double factor = matrix[j][i] / matrix[i][i];
            for (int k = 0; k < DEGREE; k++) {
                I[j][k] -= I[i][k] * factor;
                matrix[j][k] -= matrix[i][k] * factor;
            }
        }
        long double factor = matrix[i][i];
        for (int k = 0; k < DEGREE; k++) {
            I[i][k] /= factor;
            matrix[i][k] /= factor;
        }
    }
    return I;
}

int main() {
    long double* free = malloc(sizeof(long double*)*DEGREE);
    long double* sol = malloc(sizeof(long double*)*DEGREE);
    long double **system, **inverted;

    system = malloc(sizeof(long double*)*DEGREE);
    for (int i = 0; i < DEGREE; i++)
        system[i] = malloc(sizeof(long double)*DEGREE);

    system_init(system, free);
    inverted = invert(system);
    sol = multiply(inverted, free);
    print_vector(sol);
    return 0;
}