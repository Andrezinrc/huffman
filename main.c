#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char* argv[]){
    // ./huffman compress in out
    // ./huffman decompress in out
    if (argc<4) {
        printf("Uso:\n");
        printf("  %s compress <arquivo_entrada> <arquivo_saida.adr>\n", argv[0]);
        printf("  %s decompress <arquivo.adr> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    const char* mode = argv[1];
    const char* input = argv[2];
    const char* output = argv[3];

    if (strcmp(mode, "compress")==0) {
        if (compressToFile(input, output)) {
            printf("Arquivo '%s' compactado para '%s'.\n", input, output);
        } else {
            printf("Erro ao compactar '%s'.\n", input);
        }
        return 0;
    }

    if(strcmp(mode, "decompress")==0) {
        if(decompressFromFile(input, output)) {
            printf("Arquivo '%s' descompactado para '%s'.\n", input, output);
        } else {
            printf("Erro ao descompactar '%s'.\n", input);
        }
        return 0;
    }

    printf("Modo inválido: %s\n", mode);
    printf("Use 'compress' ou 'decompress'.\n");
    return 1;
}
