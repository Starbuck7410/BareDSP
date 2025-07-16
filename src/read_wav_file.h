#include <stdint.h>
#include <stdio.h>
#include "types.h"

int read_wav_header(FILE * wave, uint32_t * header_output);
int16_t * read_m16_wav_contents(FILE * wave, uint32_t * header);
void read_m16_wav_file(struct audio_m16_t * audio, FILE * wave);