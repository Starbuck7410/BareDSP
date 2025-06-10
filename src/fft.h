#include <stdlib.h>
#include <stdint.h>
#include "types.h"

cdouble_t cmplx_multiply(cdouble_t x, cdouble_t y);
cdouble_t cis(uint16_t theta);
cdouble_t cmplx_add(cdouble_t x, cdouble_t y);
void cmplx_print(cdouble_t number);
cdouble_t cmplx_normalize(cdouble_t number);
double cmplx_abs(cdouble_t number);
cdouble_t * real_dft(int16_t * data , uint32_t N);
cdouble_t * real_full_dft(int16_t * data , uint32_t N);
void export_ft(cdouble_t * X, int N, char * filename);
void export_data(int16_t * X, int N, char * filename);