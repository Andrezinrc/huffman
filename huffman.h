#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

typedef struct HuffmanNode {
    unsigned char byte;      // valor do byte (0–255)
    int freq;                // frequência do byte
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

typedef struct {
    unsigned char bits[256];  // sequência de bits, armazenada como 0 ou 1
    int length;               // quantidade de bits válidos
} HuffmanCode;

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

/*
 * Gera os códigos de Huffman percorrendo a árvore.
 */
void generateCodes(HuffmanNode* n, HuffmanCode* table, unsigned char* path, int depth);

/*
 * Preenche a tabela de códigos de Huffman a partir da raiz.
 */
void buildCodeTable(HuffmanNode* root, HuffmanCode* table);

/*
 * Calcula quantos bits serão necessários para codificar o texto.
 */
int calculateEncodedSize(const unsigned char* text, int length, HuffmanCode* table);

/*
 * Codifica um texto usando a tabela de Huffman.
 * Recebe o texto original, seu tamanho e a tabela de códigos.
 * Retorna um buffer com os bits comprimidos e preenche outSizeBytes
 * com o tamanho em bytes do buffer gerado.
 */
unsigned char* encode(const unsigned char* text, size_t length,
                      HuffmanCode* table, int* outSizeBytes);

#endif
