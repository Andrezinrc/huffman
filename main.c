#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(){
    const unsigned char* text = "aaaaaabbbbbbccccc";
    int* f = countFrequency(text, strlen((const char*)text));
    
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
            
            /*printf("byte: %3d (%c) aparece %d vezes\n", i,
            (i >= 32 && i<=126) ? i : '.' ,f[i]);
            */
        }
    }
    
    int m = findMinNode(nodes,count);
    printf("Menor freq = %d (byte = %c)\n",
       nodes[m]->freq,
       nodes[m]->byte);
    
    free(f);
    return 0;
}





