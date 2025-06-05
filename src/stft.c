#include "types.h"
#include "fft.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


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
    printf("%.2f\n", stft[0]);
    return stft;
}


int export_stft(double * X, uint32_t rows, uint32_t cols, char * filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            fprintf(fp, "%.6f", X[i * cols + j]); // Adjust precision as needed
            if (j < cols - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}