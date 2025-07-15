#include "types.h"
#include <stdint.h>

int export_stft(struct stft_t stft, char * filename);
int export_chromagram(struct chromagram_t chromagram, char * filename);
void real_stft(struct stft_t * stft, struct audio_u16_t audio, struct window_t window);
void generate_chromagram(struct chromagram_t * chromagram, struct stft_t stft);