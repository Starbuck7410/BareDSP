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

cdouble_t cmplx_magnify(double magnitude, cdouble_t number){
    cdouble_t out = {number.re, number.im};
    out.re *= magnitude;
    out.im *= magnitude;
    return out;
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

int is_power_of_two(uint32_t number){
    return (number & (number - 1)) == 0;
}



void fft_recursive(cdouble_t * X, uint32_t N) {
    if (N <= 1) return;

    cdouble_t * even = malloc((N / 2) * sizeof(cdouble_t));
    cdouble_t * odd  = malloc((N / 2) * sizeof(cdouble_t));
    cdouble_t * oddbefore = odd;

    if(even == NULL || odd == NULL){
        printf("ERROR: fft_recursive() - Failed to allocate memory.\n");
        return;
    }

    for (int i = 0; i < N / 2; i++) {
        even[i] = X[2 * i];
        odd[i]  = X[2 * i + 1];
    }

    fft_recursive(even, N/2);
    fft_recursive(odd, N/2);

    for (int k = 0; k < N/2; k++) {
        cdouble_t exp = cis((double) -2 * M_PI * k / N);
        cdouble_t adj_odd = cmplx_multiply(exp, odd[k]);
        X[k] = cmplx_add(even[k], adj_odd);
        adj_odd = cmplx_magnify(-1, adj_odd);
        X[k + N/2] = cmplx_add(even[k], adj_odd);
    }

    free(even);
    free(odd);
}

cdouble_t * real_fft(int16_t * data, uint32_t N){
    cdouble_t * X = (cdouble_t * ) malloc((N) * sizeof(cdouble_t));
    for(size_t i = 0; i < N; i++){
        X[i].re = ((double) data[i]) / M_NORM;
        X[i].im = 0;
    }  
    fft_recursive(X, N);

    return X;


}

cdouble_t * real_dft(int16_t * data , uint32_t N){
    if(N < 1){
        printf("ERROR: real_dft() - N cannot be 0.\n");
        return NULL;
    }
    cdouble_t * X;
    
    if(is_power_of_two(N)){
        X = real_fft(data, N);
    }else{

        X = (cdouble_t * ) malloc((N / 2) * sizeof(cdouble_t));
        cdouble_t * X;
        for(size_t i = 0; i < N/2; i++){
            X[i].re = 0;
            X[i].im = 0;
        }
        for (int k = 0; k < N/2; k++){
            for (int i = 0; i < N; i++){
                double x_n = ((double) data[i]) / M_NORM; 
                cdouble_t exp = cis(-i * 2 * M_PI * ((double) k)/N);
                X[k] = cmplx_add(X[k], cmplx_magnify(x_n, exp));
            }
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