#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(){
    const unsigned char* text = "aaaaaabbbbbbccccc";
    int* f = countFrequency(text, strlen((const char*)text));
    
    HuffmanNode* root = buildHuffmanTree(f);
    HuffmanCode table[256];
    buildCodeTable(root,table);
    
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
    
    /* TESTE 
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





