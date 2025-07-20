#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

// estrutura do nó da árvore de huffman
typedef struct Node {
    unsigned char character;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

// lê  um arquivo e conta a frequência de cada byte
int* CountFrequency(const char fileName[]);

// Cria e inicializa um novo nó da árvore de Huffman com caractere e frequência
Node* createNode(unsigned char character, int frequency);

// Função de comparção usada para ordenar os nós por frequência
int compareNode(const void* a, const void* b);

// Gera uma lista de nós a partir do array de frequências
int generateNodeList(int* frequency, Node* nodeList[]);

// Constrói a árvore de Huffman a partir de uma lista de nós com caracteres e suas frequências
Node* buildHuffmanTree(Node* nodes[], int count);

// Libera toda a árvore Huffman recursivamente
void freeTree(Node* root);

/*
Gera recursivamente os codigos binarios para cada caractere, percorrendo a árvore de huffman-
cada caminho da raiz até uma folha forma um código (esquerda = '0', direita = '1')
*/
void generateCodes(Node* root, char* path, int depth, char* codes[256]);

// Gera o nome de saída com extensão .adr a partir do nome do arquivo de entrada
// Exemplo: "arquivo.txt" -> "arquivo.adr"
void getAdrFilename(const char* inputPath, char* outputPath, size_t size);

// Compacta um unico arquivo e grava no .adr com caminho relativo e metadados
void compressSingleFileToStream(const char* filePath, const char* relativePath, FILE* output);

// Lê e descompacta um arquivo codificado com Huffman do arquivo .adr
void decompressSingleFileFromStream(FILE* input);

// Exibe o tamanho original e comprimido de dois arquivos e calcula a economia percentual
void showFileSizeComparison(const char* originalPath, const char* compressedPath);

#endif
