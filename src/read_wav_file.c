#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define FRAME_SIZE 4096

uint32_t read_u32_le(const uint8_t *p) {
    return (p[0]) | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

uint32_t read_u32_be(const uint8_t *p) {
    return (p[3]) | (p[2] << 8) | (p[1] << 16) | (p[0] << 24);
}

uint16_t read_u16_le(const uint8_t *p) {
    return (p[0]) | (p[1] << 8);
}

uint16_t read_u16_be(const uint8_t *p) {
    return (p[1]) | (p[0] << 8);
}



int16_t * read_wav_contents(FILE * wave) {
    uint8_t header[44];
    fread(header, 1, sizeof(header), wave);

    int is_big_endian = 0;
    if (memcmp(header, "RIFF", 4) == 0) {
        is_big_endian = 0;
    } else if (memcmp(header, "RIFX", 4) == 0) {
        is_big_endian = 1;
    } else {
        fprintf(stderr, "Not a WAV file.\n");
        return 0;
    }

    // Choose function pointers for endian-aware reading
    uint32_t (*read_u32)(const uint8_t *) = is_big_endian ? read_u32_be : read_u32_le;
    uint16_t (*read_u16)(const uint8_t *) = is_big_endian ? read_u16_be : read_u16_le;

    uint32_t chunk_size   = read_u32(header + 4);
    uint16_t audio_format = read_u16(header + 20);
    uint16_t num_channels = read_u16(header + 22);
    uint32_t sample_rate  = read_u32(header + 24);
    uint16_t bits_per_sample = read_u16(header + 34);

    // printf("Endian: %s\n", is_big_endian ? "Big" : "Little");
    // printf("Chunk size: %u\n", chunk_size);
    // printf("Audio format: %u\n", audio_format);
    printf("Channels: %u\n", num_channels);
    printf("Sample rate: %u\n", sample_rate);
    printf("Bits per sample: %u\n", bits_per_sample);
    
    // Handle unhandled cases
    if(bits_per_sample != 16){
        printf("This software currently only supports 16 bit samples.");
        return 0;
    }
    if(audio_format != 1){
        printf("This software currently only supports uncompressed PCM audio.");
        return 0;
    }
    
    uint16_t max_frame = 1 + chunk_size / (FRAME_SIZE * sizeof(int16_t));
    int16_t * data = (int16_t *)malloc(max_frame * FRAME_SIZE * sizeof(int16_t));

    for (uint16_t frame = 0; frame < max_frame; frame++){

        uint16_t read_items = fread(data + FRAME_SIZE * frame, 2, FRAME_SIZE, wave);
        
        if(read_items == 0){
            perror("Error reading from file!");
            return 0;
        }
        
        // printf("read %u items\n", read_items);

        
        for(uint16_t i = 0; i < read_items; i++){
            int16_t item_to_convert = data[FRAME_SIZE * frame + i];
            data[FRAME_SIZE * frame + i] = ((item_to_convert << 8) & 0xFF00) | ((item_to_convert >> 8) & 0x00FF);
        }
    }
    
    return data;
    
}