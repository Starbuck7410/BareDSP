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
    int16_t * data = read_wav_contents(f);
    printf("Random sample says: %d\n", data[50000]);
    free(data);
    fclose(f);
}
