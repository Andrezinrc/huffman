#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

HuffmanNode* buildHuffmanTree(int* frequencies){
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
        // Encontra dois nós de menor frequencia
        int i=findMinNode(nodes, nNodes);
        HuffmanNode* min1=nodes[i];
        nodes[i]=nodes[--nNodes];
        
        int j=findMinNode(nodes, nNodes);
        HuffmanNode* min2=nodes[j];
        nodes[j]=nodes[--nNodes];
        
        // Cria nó pai
        HuffmanNode* parent = createNode(0, min1->freq + min2->freq, min1, min2);
        nodes[nNodes++]=parent;
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

void generateCodes(HuffmanNode* n, HuffmanCode* table, unsigned char* path, int depth)
{
    if (!n) return;

    // Se é folha -> salva o código
    if (!n->left && !n->right) {
        table[n->byte].length = depth;
        for (int i=0;i<depth;i++) {
            table[n->byte].bits[i] = path[i];
        }
        return;
    }

    if(n->left){
        // Vai para a esquerda (0)
       path[depth]=0;
       generateCodes(n->left, table, path, depth + 1);
    }
    if(n->right){
        // Vai para a direita (1)
       path[depth]=1;
       generateCodes(n->right, table, path, depth + 1);
    }
}

void buildCodeTable(HuffmanNode* root, HuffmanCode* table){
    unsigned char path[256];

    // Zera a tabela
    for (int i=0;i<256;i++)
        table[i].length=0;

    generateCodes(root,table,path,0);
}

int calculateEncodedSize(const unsigned char* text, int length, HuffmanCode* table){
    int totalBits=0;
    
    for(size_t i=0;i<length;i++){
        unsigned char c=text[i];
        totalBits+=table[c].length;
    }
    return totalBits;
}




