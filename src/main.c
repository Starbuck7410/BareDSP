#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "read_wav_file.h"
#include "fft.h" 
#include "stft.h"


int main(int argc, char ** argv){
    FILE * source = fopen(argv[1], "rb");
    if (!source) {
        perror("fopen");
        return 0;
    }
    
    struct audio_m16_t audio;
    read_m16_wav_file(& audio, source);
    
    fclose(source);


    printf("Sample rate: %u\n", audio.rate);
    printf("Sample count: %u\n", audio.length);
    
    int winsize = 8192;
    int fade = winsize / 3; // The fade is defined as samples to fade-in and fade-out, has to be less than 0.5 winsize
    int winhop = winsize / 3;
    int len = audio.length / winhop;
    int sample_rate = audio.rate;
    
    struct window_t window = {
        .data = NULL,
        .length = winsize,
        .fade = fade,
        .hop = winhop,
        .type = "blkmh"
    };

    
    struct stft_t stft;
    real_stft(& stft, audio, window );
    // median_harmonic_filter(&stft, 5);
    export_stft(stft, "stft.csv");

    
    struct chromagram_t chromagram;
    generate_chromagram(& chromagram, stft);
    export_chromagram(chromagram, "chroma.csv");
    free(chromagram.data);
    
    
    free(stft.data);
    free(audio.data);
}
