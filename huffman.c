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

HuffmanNode* createNode(unsigned char byte, int freq, HuffmanNode* left, HuffmanNode* right){
    HuffmanNode* n = malloc(sizeof(HuffmanNode));
    n->byte=byte;
    n->freq=freq;
    n->left=left;
    n->right=right;
    return n;
}



