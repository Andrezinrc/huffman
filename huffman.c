#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int* countFrequency(const unsigned char* text, size_t length){
    int* f = calloc(256, sizeof(int));
    for(size_t i=0;i<length;i++){
        unsigned char c = text[i];
        f[c]++;
    }
    return f;
}



