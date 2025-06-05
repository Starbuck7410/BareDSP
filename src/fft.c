#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include <math.h>



cdouble_t cmplx_multiply(cdouble_t x, cdouble_t y){
    cdouble_t product = {0,0};
    product.re = x.re * y.re - x.im * y.im;
    product.im = x.re * y.im + y.re * x.im;
    return product;

}

cdouble_t cmplx_add(cdouble_t x, cdouble_t y){
    cdouble_t sum = {0,0};
    sum.re = x.re + y.re;
    sum.im = x.im + y.im;
    return sum;
}

cdouble_t cis(double theta){
    cdouble_t cis = {0, 0};
    cis.re = cos(theta);
    cis.im = sin(theta);
    return cis;
}


double cmplx_abs(cdouble_t number){
    return sqrt(pow(number.re, 2) + pow(number.im, 2));
}

cdouble_t cmplx_normalize(cdouble_t number){
    cdouble_t normalized = {0.0, 0.0};
    double abs = cmplx_abs(number);
    normalized.re = number.re / abs;
    normalized.im = number.im / abs;
    return normalized;
}

void cmplx_print(cdouble_t number){
    printf("%.3f + %.3fi\n", number.re, number.im);
    return;
}


cdouble_t * real_dft(int16_t * data , uint32_t N){
    cdouble_t * X = (cdouble_t * ) malloc(N * sizeof(cdouble_t));
    for (int k = 0; k < N; k++){
        for (int i = 0; i < N; i++){
            cdouble_t x_n = {((double) data[i]) / M_NORM, 0}; 
            cdouble_t exp = cis(-i * 2 * M_PI * ((double) k)/N);
            X[k] = cmplx_add(X[k], cmplx_multiply(exp, x_n));
        }
    }
    return X;
}

void export_ft(cdouble_t * X, int N, char * filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("failed to export transform file");
        return;
    }

    for (int i = 0; i < N; i++) {
        double mag = cmplx_abs(X[i]);
        fprintf(f, "%d,%.10f\n", i, mag);
    }

    fclose(f);
}

void export_data(int16_t * X, int N, char * filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("failed to export transform file");
        return;
    }

    for (int i = 0; i < N; i++) {
        fprintf(f, "%d,%.d\n", i, X[i]);
    }

    fclose(f);
}