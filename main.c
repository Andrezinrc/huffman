#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(){
    const unsigned char* text = "aaaaaabbbbbbccccc";
    int* f = countFrequency(text, strlen((const char*)text));
    
    for(int i=0;i<256;i++){
        if(f[i]>0){
            
            
            HuffmanNode* n = createNode(i,f[i],NULL,NULL);
    printf("Node criado: byte=%c freq=%d\n", n->byte, n->freq);
            
            /*printf("byte: %3d (%c) aparece %d vezes\n", i,
            (i >= 32 && i<=126) ? i : '.' ,f[i]);
            */
        }
    }
    free(f);
    return 0;
}
