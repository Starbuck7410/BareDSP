#include "types.h"
#include "fft.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double * real_stft(int16_t * data, uint32_t window_size, uint16_t hop_size, uint32_t N){
    double * stft = malloc(((N/hop_size) + 1) * window_size * sizeof(double));
    
    for(int t = 0; t < N/hop_size; t++){
        uint32_t offset = t * hop_size;
        cdouble_t * fft = real_dft(data + offset, window_size);
        for (int i = 0; i < window_size; i++) {
            stft[t * window_size + i] = cmplx_abs(fft[i]);
        }
    }
    return stft;
}

double * generate_chromagram(double * stft, uint32_t len, uint32_t window_size, uint32_t sample_rate){
    double * chromagram = (double *) malloc(12*len*sizeof(double));
    for (uint32_t i = 0; i < 12*len; i++){
        chromagram[i] = 0.0;
    }
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 1; j < window_size/2; j++) {
            double frequency = (double) j * sample_rate / window_size;
            double item = stft[i * window_size + j];
            int note = ((int) round(12 * log2(frequency / NOTE_C)));
            int idx = note % 12;
            double weight = 1 - (12 * log2(frequency / NOTE_C) - note);
            chromagram[i * 12 + idx] += item * weight;
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