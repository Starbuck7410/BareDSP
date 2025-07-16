#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// It was hard not to make a joke about how I use linux here


void window_trapezoid(struct window_t * window){
    int window_size = (*window).length;
    int fade = (*window).fade;
    double * window_data = (double *) malloc((window_size) * sizeof(double));
    for(int i = 0; i < fade; i++){
        window_data[i] = (double) i / fade;
    }
    for(int i = fade; i < window_size - fade; i++){
        window_data[i] = 1.0;
    }
    for(int i = window_size - fade; i < window_size; i++){
        window_data[i] = (window_size - i)/fade;
    }

    (* window).data = window_data;

    return;
}

void window_tukey(struct window_t * window){
    int window_size = (*window).length;
    int fade = (*window).fade;
    double * window_data = (double *) malloc((window_size) * sizeof(double));
    for(int i = 0; i < fade; i++){
        window_data[i] = 0.5 - 0.5 * cos(M_PI * i / fade);
    }
    for(int i = fade; i < window_size - fade; i++){
        window_data[i] = 1.0;
    }
    for(int i = window_size - fade; i < window_size; i++){
        window_data[i] =  0.5 - 0.5 * cos(M_PI * (i - window_size) / fade);
    }

    (* window).data = window_data;

    return;
}

void window_blackman_harris(struct window_t * window){
    int window_size = (*window).length;
    double * window_data = (double *) malloc(window_size * sizeof(double));
    double a[4] = {
	    0.35875,
    	0.48829,
        0.14128,
        0.01168
    };
    for(int i = 0; i < window_size; i++){
        window_data[i] = a[0];  
        window_data[i] -= a[1] * cos(2 * M_PI * i / window_size);
        window_data[i] += a[2] * cos(4 * M_PI * i / window_size);
        window_data[i] -= a[3] * cos(6 * M_PI * i / window_size);
    }
    (* window).data = window_data;
    return;

}



int string_hash(char * string){
    int hash = 1;
    int len = strlen(string);
    for(int i = 0; i < len - 1; i++){
        hash *= string[i] + 1 - 'a';
    }
    // printf("String: %s | Hash: %d\n", string, hash % string[len-1]);
    return hash % string[len-1];
}


void create_window(struct window_t * window){
    /*
    5 letter (lowercase) code assigned to a window type
    Currently allowed types:
    - Trapezoid "trpzd"
    - Tukey window "tukey"
    - Blackman-Harris "blkmh" (My beloved 💕)

    Check out the Desmos demonstration:
    https://www.desmos.com/calculator/idpph6sii5
    */
    int hash = string_hash((*window).type);
    switch(hash){
        case 110:
            window_tukey(window);
            break;
        case 60:
            window_trapezoid(window);
            break;
        case 0:
            window_blackman_harris(window);
            break;
        default:
            printf("create_window() - No window type specified. Falling back to trapezoid.\n");
            window_trapezoid(window);
    }
    return;
}