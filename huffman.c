#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int findMinNode(HuffmanNode** nodes, int count){
    int minIndex = -1;
    for(int i=0;i<count;i++){
        if(nodes[i]!=NULL && (minIndex == -1 || nodes[i]->freq < nodes[minIndex]->freq)){
            minIndex = i;
        }
    }
    return minIndex;
}



