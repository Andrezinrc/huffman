#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"

int* countFrequency(const unsigned char* text, size_t length){
    int* f = calloc(256, sizeof(int));
    for(size_t i=0;i<length;i++){
        unsigned char c = text[i];
        f[c]++;
    }
    return f;
}

HuffmanNode* createNode(unsigned char byte, int freq, HuffmanNode* left, HuffmanNode* right){
    HuffmanNode* n = malloc(sizeof(HuffmanNode));
    n->byte=byte;
    n->freq=freq;
    n->left=left;
    n->right=right;
    return n;
}

int findMinNode(HuffmanNode** nodes, int count){
    int minIndex = -1;
    for(int i=0;i<count;i++){
        if(nodes[i]!=NULL && (minIndex == -1 || nodes[i]->freq < nodes[minIndex]->freq)){
            minIndex = i;
        }
    }
    return minIndex;
}

HuffmanNode* buildHuffmanTree(int*  frequencies){
    
    HuffmanNode* nodes[256];
    int nNodes=0;
    
    // Cria nós folha para cada byte que aparece
    for(int i=0;i<256;i++){
        if(frequencies[i]>0){
            nodes[nNodes++]=createNode((unsigned char)i,frequencies[i],NULL,NULL);
        }
    }
    
    // Combinar nós até sobrar apenas a raiz
    while(nNodes>1){
        // Encontra dois nós de menos frequencia
        int i = findMinNode(nodes, nNodes);
        HuffmanNode* min1=nodes[i];
        nodes[i]=NULL;
        
        int j = findMinNode(nodes, nNodes);
        HuffmanNode* min2=nodes[j];
        
        // Cria Nó pai
        HuffmanNode* parent = createNode(0,min1->freq + min2->freq, min1,min2);
        nodes[i]=parent;
        if(j != nNodes-1){
            nodes[j]=nodes[nNodes-1]; // Move o ultimo nó para a posição de j
        }
        nNodes--;
    }
    
    return nodes[0];
}

/*
void printTree(HuffmanNode* n, int depth) {
    if (!n) return;
    printTree(n->left, depth+1);
    for(int i=0;i<depth;i++) printf("  ");
    if(n->left==NULL && n->right==NULL)
        printf("%c (%d)\n", n->byte, n->freq);
    else
        printf("* (%d)\n", n->freq);
    printTree(n->right, depth+1);
}
*/



