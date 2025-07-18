#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "read_wav_file.h"
#include "fft.h" 
#include "stft.h"
#include "recognition.h"
#define M_CHORDS 36

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
    // median_harmonic_filter(&stft, 19);
    export_stft(stft, "stft.csv");

    
    struct chromagram_t chromagram;
    generate_chromagram(& chromagram, stft);
    export_chromagram(chromagram, "chroma.csv");


    struct note_t chord[3];
    
    
    
    struct chord_t chord_array[M_CHORDS];
    // Setup majors
    for (int i = 0; i < 12; i++){
        chord[0] = synthesize_note(i % 12);
        chord[1] = synthesize_note((i+4) % 12);
        chord[2] = synthesize_note((i+7) % 12);
        chord_array[i] = synthesize_chord(chord, 3);
    }
    // And minors
    for (int i = 12; i < 24; i++){
        chord[0] = synthesize_note(i % 12);
        chord[1] = synthesize_note((i+3) % 12);
        chord[2] = synthesize_note((i+7) % 12);
        chord_array[i] = synthesize_chord(chord, 3);
    }
    // And sus minor 2nd?
    for (int i = 24; i < 36; i++){
        chord[0] = synthesize_note(i % 12);
        chord[1] = synthesize_note((i+2) % 12);
        chord[2] = synthesize_note((i+7) % 12);
        chord_array[i] = synthesize_chord(chord, 3);
    }


    struct chord_dict_t dictionary = {
        .length = M_CHORDS,
        .threshold = 0.4,
        .chord_array = chord_array,
        .transition_mtx = NULL
    };
    
    
    printf("The chord progression played was: ");
    int last = -1;
    for (int i = 0; i < chromagram.length; i++){
        int best_idx = find_matching_chord(detect_chord_signature(chromagram, i), dictionary);
        if(best_idx != -1 && last != best_idx){
            printf("%d ", best_idx);
            last = best_idx;
        }
    }

    free(chromagram.data);
    
    
    free(stft.data);
    free(audio.data);
}
