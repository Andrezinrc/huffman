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

/*
 * Encontra o índice do nó com menor frequência
 */
int findMinNode(HuffmanNode** nodes, int count);

/*
 * Constrói a árvore de Huffman e retorna a raiz
 */
HuffmanNode* buildHuffmanTree(int* frequencies);

/*
 * Imprime a árvore de Huffman de forma recursiva
 * para visualização. depth indica a profundidade atual.
 */
void printTree(HuffmanNode* root, int depth);

#endif
