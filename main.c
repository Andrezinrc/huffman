#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(){
    const unsigned char* text = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbc";
    
    size_t len = strlen((const char*)text);
    int* f = countFrequency(text,len);
    
    HuffmanNode* root = buildHuffmanTree(f);
    HuffmanCode table[256];
    buildCodeTable(root,table);
    
    // TESTE ENCODE
    int outSize=0;
    unsigned char* compressed = encode(text,len,table,&outSize);
    
    printf("Tamanho original: %zu bytes\n", len);
    printf("Tamanho comprimido: %d bytes\n", outSize);

    printf("\nBytes comprimidos:\n");
    for (int i=0;i<outSize;i++)
        printf("0x%02X ", compressed[i]);
    printf("\n");
    
    // TESTE DECODE
    int bitCount = calculateEncodedSize(text, len, table);

    unsigned char* restored = decode(compressed, bitCount, root, len);

    printf("\nTexto descomprimido:\n%s\n", restored);


    /* TESTE
    for(int i=0;i<256;i++){
        if(table[i].length>0){
            printf("'%c' (byte %d) = ", (i>=32 && i<=126) ? i : '.', i);
            for(int j=0;j<table[i].length;j++){
                printf("%d", table[i].bits[j]);
            }
            printf("\n");
        }
    }
    
    
    
    int totalBits = calculateEncodedSize(text, strlen((const char*)text),table);
    printf("Total de bits necessários: %d\n", totalBits);
    printf("Tamanho estimado: %d bytes\n", (totalBits+7)/8);
    
    int count=0;
    for(int i=0;i<256;i++){
        if(f[i]>0) count++;
    }
    
    HuffmanNode** nodes = malloc(count * sizeof(HuffmanNode*));
    
    int idx=0;
    for(int i=0;i<256;i++){
        if(f[i]>0){
            
            HuffmanNode* n = createNode(i,f[i],NULL,NULL);
    printf("Node criado: byte=%c freq=%d\n", n->byte, n->freq);
           nodes[idx++] = n;
            
            // printTree(root, 0);
            printf("byte: %3d (%c) aparece %d vezes\n", i,
            (i >= 32 && i<=126) ? i : '.' ,f[i]);
        }
    }
    
    int m = findMinNode(nodes,count);
    printf("Menor freq = %d (byte = %c)\n",
       nodes[m]->freq,
       nodes[m]->byte);
    */
    
    free(f);
    // free(nodes);
    return 0;
}


