#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "functions.h"


int main(int argc, char ** argv){
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 0;
    }
    uint32_t header[6];
    read_wav_header(f, header);
    int16_t * data = read_wav_contents(f, header);

    printf("Channels: %u\n", header[3]);
    printf("Sample rate: %u\n", header[4]);
    printf("Bit depth: %u\n", header[5]);
    printf("Random sample says: %d\n", data[50000]);
    free(data);
    fclose(f);
}
