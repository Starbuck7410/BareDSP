#include "types.h"
#include "fft.h"
#include "windows.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void real_stft(struct stft_t * stft, struct audio_m16_t audio, struct window_t window){

    create_window(& window);
    double * stft_data = (double *) malloc((audio.length/window.hop) * (window.length / 2) * sizeof(double) );
    cdouble_t * fft;
    double * data_windowed = (double *) malloc((window.length) * sizeof(double));

    for(int t = 0; t < audio.length/window.hop; t++){
        uint32_t offset = t * window.hop;
        for(int i = 0; i < window.length; i++) {
            if (offset + i < audio.length) {
                data_windowed[i] = ((double) (audio.data[offset + i]) / M_NORM) * window.data[i];
            } else {
                data_windowed[i] = 0.0;
            }
        }
        fft = calculate_dft(data_windowed, window.length);
        for (int i = 0; i < window.length / 2; i++) {
            stft_data[t * window.length / 2 + i] = cmplx_abs(fft[i]);
        }
        free(fft);
    }
    free(window.data);
    free(data_windowed);
    
    (* stft).length = audio.length/window.hop;
    (* stft).bins = window.length/2;
    (* stft).data = stft_data;
    (* stft).rate = audio.rate;
    return;
}

double find_median(double * filter, int length){
    for (int i = 0; i < length; i++){
        for (int j = 0; j < length - 1; j++){
            if(filter[j] < filter[j+1]){
                double tmp = filter[j+1];
                filter[j+1] = filter[j];
                filter[j] = tmp; 
            } 
        }
    }
    
    return (length & 1) ? filter[length/2] : (filter[length/2 - 1] + filter[length/2]) / 2.0;
}

void median_harmonic_filter(struct stft_t * stft, int width){
    int stft_bins = (*stft).bins;
    int stft_length = (*stft).length;
    double * filtered_data = (double *) malloc(stft_bins * stft_length * sizeof(double));
    double filter[width];

    for (int i = 0; i < stft_length; i++){
        for (int j = 0; j < stft_bins; j++){
            for (int k = 0; k < width; k++){
                filter[k] = ((i + k - width/2) >= 0 && (i + k - width/2) < stft_length) ? (*stft).data[(i + k - width/2) * stft_bins + j] : 0;
            }
            filtered_data[i * stft_bins + j] = find_median(filter, width);
        }
    }
    free((*stft).data);
    (*stft).data = filtered_data;
    return;
}


void generate_chromagram(struct chromagram_t * chromagram, struct stft_t stft){
    double * chromagram_data = (double *) malloc(12 * stft.length * sizeof(double));
    for (uint32_t i = 0; i < 12 * stft.length; i++){
        chromagram_data[i] = 0;
    }
    for (uint32_t i = 0; i < stft.length; i++) {
        for (uint32_t j = 0; j < stft.bins; j++) {
            double frequency = (double) j * stft.rate / (stft.bins * 2);
            if (frequency < 20 || frequency > 5000) continue;
            
            double item = stft.data[i * stft.bins + j];
            int note = ((int) round(12 * log2(frequency / NOTE_C)));
            int idx = note % 12;
            while(idx < 0) idx += 12;
            double weight = pow(1 - (12 * log2(frequency / NOTE_C) - note), 1); // Change the power to affect the weighting
            chromagram_data[i * 12 + idx] += item * weight;
        }
    }
    (* chromagram).data = chromagram_data;
    (* chromagram).length = stft.length;

    return;
}


int export_stft(struct stft_t stft, char * filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }
    for (size_t i = 0; i < stft.length; i++) {
        for (size_t j = 0; j < stft.bins; j++) {
            fprintf(fp, "%.10f", stft.data[i * stft.bins + j]); 
            if (j < stft.bins - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}

int export_chromagram(struct chromagram_t chromagram, char * filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (size_t i = 0; i < chromagram.length; i++) {
        for (size_t j = 0; j < 12; j++) {
            fprintf(fp, "%.10f", chromagram.data[i * 12 + j]); 
            if (j < 12 - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}

