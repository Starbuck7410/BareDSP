#include "types.h"
#include <stdint.h>

int export_stft(double * X, uint32_t rows, uint32_t cols, char * filename);
double * real_stft(int16_t * data, uint32_t window_size, uint16_t hop_size, uint32_t N);