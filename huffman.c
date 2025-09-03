#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "huffman.h"

int* CountFrequency(const char fileName[]){
    int *frequency = calloc(256, sizeof(int));

    if (!frequency) {
        printf("Erro ao alocar memória para frequências.\n");
        return NULL;
    }

    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Erro ao abrir arquivo");
        free(frequency);
        return NULL;
    }

    unsigned char byte;
    // le o arquivo byte a byte
    while (fread(&byte, 1, 1, file) == 1) {
        frequency[byte]++;
    }

    fclose(file); file = NULL;

    return frequency;
}

Node* createNode(unsigned char character, int frequency){
    Node *node = (Node*)malloc(sizeof(Node));
    if(!node) return NULL;

    // inicializa os campos do nó
    node->frequency = frequency;
    node->character = character;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int compareNode(const void* a, const void* b){
    Node *nodeA = *(Node**)a; // converte o ponteiro genérico para ponteiro de Node
    Node *nodeB = *(Node**)b; // idem

    return nodeA->frequency - nodeB->frequency; // ordena pela frequência
}

int generateNodeList(int* frequency, Node* nodeList[]){
    int count = 0;

    for(int i = 0; i < 256; i++){
        if(frequency[i] > 0){
            // cria um nó com o caractere e sua frequência
            nodeList[count++] = createNode((unsigned char)i, frequency[i]);
        }
    }

    return count;
}

Node* buildHuffmanTree(Node* nodes[], int count){
    // ordena a lista de nós por frequência
    while(count > 1){
        // pega os dois nós de menor frequência
        Node *left = nodes[0];
        Node *right = nodes[1];

        // cria um novo nó combinando os dois
        Node *newNode = createNode('\0', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        // move todos os nós restantes 2 posiçoes para tras
        for(int i = 2; i < count; i++){
            nodes[i-2] = nodes[i];
        }

        // adiciona o novo nó no fim da lista
        nodes[count-2] = newNode;
        count--;
    }

    // quando restar só um nó, é a raiz da árvore
    return nodes[0];
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void generateCodes(Node* root, char* path, int depth, char* codes[256]){
    if(!root) return;

    // se for folha, salva o codigo
    if(!root->left && !root->right){
        path[depth] = '\0'; // marca o fim da string
        codes[root->character] = strdup(path); // salva copia do codigo
        return;
    }

    // esquerda = 0
    path[depth] = '0';
    generateCodes(root->left, path, depth + 1, codes);

    // direita = 1
    path[depth] = '1';
    generateCodes(root->right, path, depth + 1, codes);
}

void getAdrFilename(const char* inputPath, char* outputPath, size_t size) {
    const char* lastSlash = strrchr(inputPath, '/');
    const char* fileName = lastSlash ? lastSlash + 1 : inputPath;

    // copia o nome sem extensão
    strncpy(outputPath, fileName, size);
    char* dot = strrchr(outputPath, '.');
    if (dot) *dot = '\0';  // remove extensão

    strncat(outputPath, ".adr", size - strlen(outputPath) - 1);
}

void compressSingleFileToStream(const char* filePath, const char* relativePath, FILE* output) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", filePath);
        return;
    }

    // calcula frequência dos bytes
    int* freq = CountFrequency(filePath);
    if (!freq) {
        fclose(file);
        return;
    }

    // cria árvore de huffman e gera os codigos
    Node* nodeList[256];
    int count = generateNodeList(freq, nodeList);
    qsort(nodeList, count, sizeof(Node*), compareNode);
    Node* root = buildHuffmanTree(nodeList, count);

    char* codes[256] = {0};
    char path[256];
    generateCodes(root, path, 0, codes);

    // escreve caminho relativo
    uint16_t pathLen = (uint16_t)strlen(relativePath);
    fwrite(&pathLen, sizeof(uint16_t), 1, output);
    fwrite(relativePath, 1, pathLen, output);

    // escreve tamanho original do arquivo
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    uint32_t fileSize32 = (uint32_t)fileSize;
    fwrite(&fileSize32, sizeof(uint32_t), 1, output);

    // Escreve apenas símbolos usados
    uint16_t unique_count = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) unique_count++;
    }
    fwrite(&unique_count, sizeof(uint16_t), 1, output);
    
    // escreve apenas os símbolos que aparecem
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            uint8_t symbol = (uint8_t)i;
            uint32_t frequency = (uint32_t)freq[i];
            fwrite(&symbol, 1, 1, output);
            fwrite(&frequency, sizeof(uint32_t), 1, output);
        }
    }

    unsigned char buffer = 0;
    int bitCount = 0;
    int c;
    long bytesRead = 0;
    int lastPercent = -1;

    // codifica os dados byte a byte usando huffman e grava os bits compactados no arquivo
    while ((c = fgetc(file)) != EOF) {
        const char* code = codes[c];
        for (int i = 0; code[i] != '\0'; i++) {
            buffer <<= 1;
            if (code[i] == '1') buffer |= 1;
            bitCount++;

            /*
            quando 8 bits sao preenchidos,
            grava o byte no arquivo
            */
            if (bitCount == 8) {
                fwrite(&buffer, 1, 1, output);
                buffer = 0;
                bitCount = 0;
            }
        }
        bytesRead++;
    }

    /*
    se restaram bits nao gravados,
    preenche com zeros a direita e grava o utilmo byte
    */
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, 1, 1, output);
    }

    // libera memoria
    fclose(file);
    free(freq);
    for (int i = 0; i < 256; i++) {
        if (codes[i]) free(codes[i]);
    }
    freeTree(root);
}

void decompressSingleFileFromStream(FILE* input) {
    uint16_t pathLen;
    fread(&pathLen, sizeof(uint16_t), 1, input);

    char path[1024];
    fread(path, 1, pathLen, input);
    path[pathLen] = '\0';

    // lê o tamanho original do arquivo
    uint32_t originalSize;
    fread(&originalSize, sizeof(uint32_t), 1, input);

    // lê a tabela de frequência compactada
    uint16_t unique_count;
    fread(&unique_count, sizeof(uint16_t), 1, input);
    
    int freq[256] = {0}; // inicializa com zeros
    for (int i = 0; i < unique_count; i++) {
        uint8_t symbol;
        uint32_t frequency;
        fread(&symbol, 1, 1, input);
        fread(&frequency, sizeof(uint32_t), 1, input);
        freq[symbol] = (int)frequency;
    }

    // reconstrói a árvore de Huffman
    Node* nodeList[256];
    int count = generateNodeList(freq, nodeList);
    qsort(nodeList, count, sizeof(Node*), compareNode);
    Node* root = buildHuffmanTree(nodeList, count);

    // prepara para decodificação
    FILE* output = fopen(path, "wb");
    if (!output) {
        fprintf(stderr, "Erro ao criar arquivo de saída %s\n", path);
        freeTree(root);
        return;
    }

    Node* current = root;
    unsigned char byte;
    int written = 0;

    // decodifica bit a bit até atingir o tamanho original
    while (written < originalSize && fread(&byte, 1, 1, input) == 1) {
        for (int i = 7; i >= 0 && written < originalSize; i--) {
            int bit = (byte >> i) & 1;
            current = bit == 0 ? current->left : current->right;

            // se chegou numa folha, escreve o caractere
            if (!current->left && !current->right) {
                fputc(current->character, output);
                written++;
                current = root; // volta pro topo da árvore
            }
        }
    }

    fclose(output);
    freeTree(root);
}

void showFileSizeComparison(const char* originalPath, const char* compressedPath) {
    FILE* f1 = fopen(originalPath, "rb");
    FILE* f2 = fopen(compressedPath, "rb");

    if (!f1 || !f2) {
        printf("Erro ao abrir os arquivos para comparação de tamanho.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return;
    }

    fseek(f1, 0, SEEK_END);
    fseek(f2, 0, SEEK_END);

    long size1 = ftell(f1);
    long size2 = ftell(f2);

    fclose(f1);
    fclose(f2);

    double percent = 100.0 * (1.0 - (double)size2 / size1);

    printf("Tamanho original: %.2f KB\n", size1 / 1024.0);
    printf("Tamanho comprimido: %.2f KB\n", size2 / 1024.0);
    printf("Economia: %.2f%%\n", percent);
}
