#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "read_wav_file.h"
#include "fft.h" 
#include "stft.h"


int main(int argc, char ** argv){
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 0;
    }
    uint32_t header[6];
    read_wav_header(f, header);

    struct audio_16_t audio;
    audio.data = read_wav_contents(f, header);
    audio.length = header[1]/sizeof(uint16_t);
    audio.rate = header[4];

    printf("Channels: %u\n", header[3]);
    printf("Sample rate: %u\n", header[4]);
    printf("Bit depth: %u\n", header[5]);
    printf("Sample count: %u\n", audio.length);

    int winsize = 8192;
    int fade = winsize / 3; // The fade is defined as samples to fade-in and fade-out, has to be less than 0.5 winsize
    int winhop = winsize / 3;
    int len = audio.length / winhop;
    int sample_rate = header[4];
    
    struct window_t window = {
        .data = NULL,
        .length = winsize,
        .fade = fade,
        .hop = winhop
    };

    struct stft_t stft;

    real_stft(& stft, audio, window );
    
    export_stft(stft, "stft.csv");

    printf("Finished STFT, starting chromagram!\n");

    struct chromagram_t chromagram;

    generate_chromagram(& chromagram, stft);


    export_chromagram(chromagram, "chroma.csv");


    free(stft.data);
    // free(chromagram.data);
    free(audio.data);
    fclose(f);
}
