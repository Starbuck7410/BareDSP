#include <stdint.h>
#include <stdio.h>

int read_wav_header(FILE * wave, uint32_t * header_output);
int16_t * read_wav_contents(FILE * wave, uint32_t * header);