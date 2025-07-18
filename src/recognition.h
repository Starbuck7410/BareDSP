#include "types.h"

struct note_t synthesize_note(int id);
struct chord_t synthesize_chord(struct note_t * notes, int n);
struct chord_t construct_chord(double * notes);
double chord_similarity(struct chord_t a, struct chord_t b);
void print_chord(struct chord_t chord);
struct chord_t detect_chord_signature(struct chromagram_t chromagram, int n);
int find_matching_chord(struct chord_t chord, struct chord_dict_t dictionary);