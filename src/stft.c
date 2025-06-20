#include "types.h"
#include "fft.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



double * window_trapezoid(uint32_t window_size, uint32_t fade){
    double * window = (double *) malloc((window_size + fade) * sizeof(double));
    for(int i = 0; i < fade; i++){
        window[i] = (double) i / fade;
    }
    for(int i = fade; i < window_size - fade; i++){
        window[i] = 1.0;
    }
    for(int i = window_size - fade; i < window_size; i++){
        window[i] = (window_size - i)/fade;
    }
    return window;
}


double * real_stft(int16_t * data, uint32_t window_size, uint16_t hop_size, uint32_t fade, uint32_t N){
    double * window = window_trapezoid(window_size, fade);
    double * stft = (double *) malloc((N/hop_size) * (window_size / 2) * sizeof(double) );
    cdouble_t * fft;
    double * data_windowed = (double *) malloc((window_size) * sizeof(double));

    for(int t = 0; t < N/hop_size; t++){
        uint32_t offset = t * hop_size;
        for(int i = 0; i < window_size; i++ ){
            data_windowed[i] = ((double) data[offset + i] / M_NORM) * window[i];
        }
        fft = calculate_dft(data_windowed, window_size);
        for (int i = 0; i < window_size/2; i++) {
            stft[t * window_size/2 + i] = cmplx_abs(fft[i]);
        }
        free(fft);
    }
    free(window);
    free(data_windowed);
    return stft;
}

double * generate_chromagram(double * stft, uint32_t len, uint32_t window_size, uint32_t sample_rate){
    double * chromagram = (double *) malloc(12*len*sizeof(double));
    for (uint32_t i = 0; i < 12*len; i++){
        chromagram[i] = 0;
    }
    for (uint32_t i = 0; i < len; i++) {
        for (uint32_t j = 0; j < window_size/2; j++) {
            double frequency = (double) j * sample_rate / window_size;
            if (frequency < 20 || frequency > 5000) continue;
            // printf("Accessing stft[%d][%d] (%d)\n", i, j, i * window_size/2 + j);
            
            double item = stft[i * window_size/2 + j];
            int note = ((int) round(12 * log2(frequency / NOTE_C)));
            int idx = note % 12;
            while(idx < 0) idx += 12;
            double weight = pow(1 - (12 * log2(frequency / NOTE_C) - note), 1);
            // printf("Accessing chromagram at [%d][%d] (%d)\n", i, idx, i * 12 + idx);
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
        for (size_t j = 0; j < N/2; j++) {
            fprintf(fp, "%.10f", X[i * N/2 + j]); 
            if (j < N/2 - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}

int export_chromagram(double * X, uint32_t len, char * filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; j < 12; j++) {
            fprintf(fp, "%.10f", X[i * 12 + j]); 
            if (j < 12 - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}

