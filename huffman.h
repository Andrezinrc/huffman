#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

typedef struct HuffmanNode {
    unsigned char byte;      // valor do byte (0–255)
    int freq;                // frequência do byte
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

/*
 * Conta a frequência de cada byte.
 */
int* countFrequency(const unsigned char* text, size_t length);

/*
 * Cria um nó da árvore de Huffman (folha ou interno).
 */
HuffmanNode* createNode(unsigned char byte, int freq,
                        HuffmanNode* left, HuffmanNode* right);

#endif
