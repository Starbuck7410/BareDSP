
#include "types.h"
#include "fft.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double * real_stft(int16_t * data, uint32_t window_size, uint16_t hop_size, uint32_t N){
    // double [(N/hop_size) + 1][window_size] stft;
    int W = window_size;
    double * stft = malloc(((N/hop_size) + 1) * window_size * sizeof(double));
    
    for(int t = 0; t < N/hop_size; t++){
        uint32_t offset = t * hop_size;
        cdouble_t * fft = real_dft(data + offset, window_size);
        for (int i = 0; i < window_size; i++) {
            stft[t * W + i] = cmplx_abs(fft[i]);
        }
    }
    return stft;
}

double * generate_chromagram(double * stft, uint32_t len, uint32_t window_size, uint32_t sample_rate){
    double * chromagram = (double *) malloc(12*len*sizeof(double));
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < window_size; j++) {
            double item = stft[i * window_size + j];
            int idx = ((int) round(log2((j * sample_rate) / (window_size * NOTE_C)))) % 12;
            // printf("%d", idx);
            chromagram[i * 12 + idx] += item;
        }
    }
    return chromagram;
}


int export_stft(double * X, uint32_t len, uint32_t N, char * filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < N; j++) {
            fprintf(fp, "%.6f", X[i * N + j]); // Adjust precision as needed
            if (j < N - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}