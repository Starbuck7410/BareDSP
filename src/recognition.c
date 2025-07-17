#include "types.h"
#include <math.h>
#include <stdio.h>

int mod(int a, int b)
{
    int mod = a % b;
    return mod < 0 ? mod + b : mod;
}

struct note_t synthesize_note(int id){
    struct note_t note;
    note.id = id;
    double alpha = M_ALPHA; // This essentially depends on the instrument
    double c_note[12] = {
        1 + M_ALPHA + pow(M_ALPHA, 3) + pow(M_ALPHA, 7),
        0,
        0,
        0,
        pow(M_ALPHA, 4),
        0,
        0,
        pow(M_ALPHA, 2) + pow(M_ALPHA, 7),
        0,
        0,
        pow(M_ALPHA, 6)
    };
    for(int i = 0; i < 12; i++){
        note.signature[i] = c_note[mod(i - id, 12)];
    }
    return note;
}

struct chord_t synthesize_chord(struct note_t * notes, int n){
    struct chord_t chord;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 12; j++){
            chord.signature[j] += notes[i].signature[j]; 
        }
        chord.notes[notes[i].id] = 1;
    }
    return chord;
}

struct chord_t construct_chord(double * notes){
    struct chord_t chord;
    for(int i = 0; i < 12; i++){
        chord.signature[i] += notes[i]; 
        chord.notes[i] = -1; 
    }
    return chord;
}

double chord_similarity(struct chord_t a, struct chord_t b){
    double scale_a = 0;
    double scale_b = 0;
    double similarity = 0;
    for(int i = 0; i < 12; i++){
        similarity += a.signature[i] * b.signature[i];
        scale_a += a.signature[i] * a.signature[i];
        scale_b += b.signature[i] * b.signature[i];
    }
    scale_a = sqrt(scale_a);
    scale_b = sqrt(scale_b);
    return similarity / (scale_a * scale_b);
}

struct chord_t detect_chord_signature(struct chromagram_t chromagram, int n){
    if(n > chromagram.length){
        printf("detect_chord_signature() - n longer than chromagram length.");
        return (struct chord_t) {0};
    }
    struct chord_t chord;
    for (int i = 0; i < 12; i++){
        chord.signature[i] = chromagram.data[n * 12 + i];
        chord.notes[i] = -1;
    }
    return chord;
}

void print_chord(struct chord_t chord){
    printf("Chord is:\n");
    for(int i = 0; i< 12; i++){
        printf("%d ", chord.notes[i]);
    }
    printf("\nWith signature:\n");
    for(int i = 0; i< 12; i++){
        printf("%.3f ", chord.signature[i]);
    }
    printf("\n");
    return;
}