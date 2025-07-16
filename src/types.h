#include <stdint.h>
#define M_PI 3.14159265358979323846
#define M_NORM 32768
#define NOTE_C 261.63


typedef struct cdouble_t{
    double re;
    double im;

} cdouble_t;

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