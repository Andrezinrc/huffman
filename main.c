#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>

#include "huffman.h"

int main(int argc, char* argv[]) {
    //printf("Hello, World!\n");

    if (argc != 3) {
        printf("Uso: %s compress <arquivo.txt>\n", argv[0]);
        printf("     %s decompress <arquivo.adr>\n", argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *file = argv[2];

    int* freq = CountFrequency(file);

    if (!freq) return 1;

    for (int i = 0; i < 256; i++){
        if (freq[i] > 0){
            //printf("Byte %3d => %d\n", i, freq[i]);
            Node* node = createNode((unsigned char)i, freq[i]);
            if (node) {
                /*printf("Nó criado: caractere '%c' (byte %d), frequência %d\n", 
                       node->character,
                       node->character, 
                       node->frequency); */
                free(node);
            } else {
                printf("Erro ao criar nó para byte %d\n", i);
            }
        }
    }
    /*
    Node* nodes[3];
    nodes[0] = createNode('A', 5);
    nodes[1] = createNode('B', 2);
    nodes[2] = createNode('C', 8);

    // ordena o array com qsort usando sua função
    qsort(nodes, 3, sizeof(Node*), compareNode);
    */

    Node* nodeList[256];
    int count = generateNodeList(freq, nodeList);
    qsort(nodeList, count, sizeof(Node*), compareNode);

    // imprime pra verificar se ordenou corretamente
    for (int i = 0; i < count; i++) {
        //printf("'%c': %d\n", nodeList[i]->character, nodeList[i]->frequency);
    }

    // constrói a árvore de Huffman
    Node* root = buildHuffmanTree(nodeList, count);
    if (root) {
        //printf("\nRaiz da árvore: frequência total = %d\n", root->frequency);
    }

    char* codes[256] = {0};
    char path[256];

    generateCodes(root, path, 0, codes);

    //printf("\nCódigos gerados:\n");
    for (int i = 0; i < 256; i++) {
        if (codes[i]) {
            //printf("'%c': %s\n", (char)i, codes[i]);
            free(codes[i]);
        }
    }

    if (strcmp(mode, "compress") == 0) {
        char outputPath[256];
        getAdrFilename(file, outputPath, sizeof(outputPath));

        FILE* out = fopen(outputPath, "wb");
        if (!out) {
            fprintf(stderr, "Erro ao criar %s\n", outputPath);
            return 1;
        }

        compressSingleFileToStream(file, file, out);
        fclose(out);
        printf("Arquivo compactado como %s\n", outputPath);

        // exibe os tamanhos antes e depois da compressão e mostra a economia obtido
        showFileSizeComparison(file, outputPath);
    } 
    else if (strcmp(mode, "decompress") == 0) {
        FILE* in = fopen(file, "rb");
        if (!in) {
            printf("Erro ao abrir %s\n", file); 
            return 1;
        }

        decompressSingleFileFromStream(in);
        fclose(in);
        printf("Arquivo descompactado com sucesso!\n");
    } 
    else {
        printf("Comando inválido. Use 'compress' ou 'decompress'.\n");
        return 1;
    }

    freeTree(root);
    free(freq);

    return 0;
}
