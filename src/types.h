#include <stdint.h>
#define M_PI 3.14159265358979323846
#define M_NORM 32768
#define NOTE_C 261.63
#define M_ALPHA 0.3

typedef struct cdouble_t{
    double re;
    double im;

} cdouble_t;

// Audio data (Mono 16 bit)
struct audio_m16_t{
    int16_t * data;
    int length;
    int rate;
};

struct stft_t {
    double * data;
    int bins;
    int length; 
    int rate;
};

struct chromagram_t {
    double * data;
    int length; 
};

struct window_t {
    double * data;
    char type[6];
    int fade;
    int length; 
    int hop;
};

struct note_t {
    double signature[12];
    int id; // Starts with 0 for C, up to 11 for B
};

struct chord_t {
    double signature[12];
    int notes[12]; // 0 is inactive, 1 is active, -1 is unknown
};

struct chord_dict_t {
    int length;
    double threshold;
    struct chord_t * chord_array;
    char ** chord_names;
    double * transition_mtx;
};