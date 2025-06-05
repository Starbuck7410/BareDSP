#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "read_wav_file.h"
#include "fft.h" 


int main(int argc, char ** argv){
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 0;
    }
    uint32_t header[6];
    read_wav_header(f, header);
    int16_t * data = read_wav_contents(f, header);
    
    uint32_t N = header[1]/2;
    printf("Channels: %u\n", header[3]);
    printf("Sample rate: %u\n", header[4]);
    printf("Bit depth: %u\n", header[5]);
    printf("Sample count: %u\n", N);



    cdouble_t * X = real_dft(data, N);
    free(X);
    export_ft(X, N/2, "dft.csv");
    export_data(data, N, "audio.csv");
    free(data);
    fclose(f);
}
