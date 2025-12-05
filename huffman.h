#ifndef HUFFMAN_H
#define HUFFMAN_H

/*
 * Implementação simples de compressão Huffman.
 *
 * Fornece:
 *  - contagem de frequência
 *  - construção da árvore
 *  - geração da tabela de códigos
 *  - codificação/decodificação de buffers
 *  - compressão/descompressão de arquivos .adr
 *
 * .adr armazenam:
 *  - tabela de frequências
 *  - tamanho original
 *  - bitstream comprimido
 */

#include <stddef.h>

typedef struct HuffmanNode {
    unsigned char byte;
    int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct {
    unsigned char bits[256];
    int length;
} HuffmanCode;

int* countFrequency(const unsigned char *text, size_t length);

HuffmanNode* createNode(unsigned char byte, int freq,
                        HuffmanNode *left, HuffmanNode *right);

int findMinNode(HuffmanNode **nodes, int count);
HuffmanNode* buildHuffmanTree(int *frequencies);

void printTree(HuffmanNode *root, int depth);

void generateCodes(HuffmanNode *n, HuffmanCode *table,
                   unsigned char *path, int depth);

void buildCodeTable(HuffmanNode *root, HuffmanCode *table);

int calculateEncodedSize(const unsigned char *text, int length,
                         HuffmanCode *table);

unsigned char* encode(const unsigned char *text, size_t length,
                      HuffmanCode *table, int *outSizeBytes);


unsigned char* decode(const unsigned char *data, int bitCount,
                      HuffmanNode *root, int originalSize);



int compressToFile(const char *inputPath, const char *outputAdrPath);
int decompressFromFile(const char *adrPath, const char *outputPath);

#endif
