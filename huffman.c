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

unsigned char* encode(const unsigned char* text, size_t length,
                      HuffmanCode* table, int* outSizeBytes){
    int totalBits = calculateEncodedSize(text,length,table);
    int totalBytes = (totalBits + 7)/8;  // Arredonda pra cima

    unsigned char* out = calloc(totalBytes, 1);

    int bitPos=0;  // Posiçao global no fluxo de bits

    for (size_t i=0;i<length;i++) {
        unsigned char c=text[i];
        HuffmanCode code=table[c];
        for (int b=0;b<code.length;b++) {
            int byteIdx = bitPos / 8;
            int bitInByte = 7 -(bitPos % 8);
            if (code.bits[b]==1)
                out[byteIdx] |= (1 << bitInByte);
            bitPos++;
        }
    }

    *outSizeBytes=totalBytes;
    return out;
}

unsigned char* decode(const unsigned char* data, int bitCount, HuffmanNode* root, int originalSize){
    unsigned char* out = malloc(originalSize);
    int outPos=0;

    HuffmanNode* cur = root;

    for (int bitPos=0;bitPos<bitCount;bitPos++) {

        int byteIdx=bitPos / 8;
        int bitInByte = 7 -(bitPos % 8);

        int bit = (data[byteIdx] >> bitInByte) & 1;

        // Anda na árvore
        if (bit==0)
            cur=cur->left;
        else
            cur=cur->right;

        // Chegou na folha -> escreve char
        if (!cur->left && !cur->right) {
            out[outPos++] = cur->byte;
            cur=root; // Volta para raiz
            if (outPos == originalSize)
                break;
        }
    }

    return out;
}

int compressToFile(const char* inputPath, const char* outputAdrPath){
    FILE* in = fopen(inputPath, "rb");
    if (!in) return 0;

    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    fseek(in, 0, SEEK_SET);

    unsigned char* buffer = malloc(size);
    fread(buffer, 1, size, in);
    fclose(in);

    int* freq = countFrequency(buffer, size);

    HuffmanNode* root = buildHuffmanTree(freq);
    HuffmanCode table[256];
    buildCodeTable(root, table);

    int encodedSizeBytes;
    unsigned char* encoded = encode(buffer, size, table, &encodedSizeBytes);

    FILE* out = fopen(outputAdrPath, "wb");
    if (!out) return 0;

    fwrite(freq, sizeof(int), 256, out);
    fwrite(&size, sizeof(int), 1, out);

    const char* dot = strrchr(inputPath, '.');
    const char* ext = (dot ? dot : "");
    int extLen = strlen(ext);

    fwrite(&extLen, sizeof(int), 1, out);
    fwrite(ext, 1, extLen, out);
    fwrite(encoded, 1, encodedSizeBytes, out);

    fclose(out);

    free(buffer);
    free(freq);
    free(encoded);

    return 1;
}

int decompressFromFile(const char* adrPath, const char* outputBase){
    FILE* f = fopen(adrPath, "rb");
    if (!f) return 0;

    int freq[256];
    fread(freq, sizeof(int), 256, f);

    HuffmanNode* root = buildHuffmanTree(freq);

    int originalSize;
    fread(&originalSize, sizeof(int), 1, f);

    int extLen;
    fread(&extLen, sizeof(int), 1, f);

    char ext[32];
    fread(ext, 1, extLen, f);
    ext[extLen] = '\0';

    fseek(f, 0, SEEK_END);
    long endPos = ftell(f);
    long dataSize = endPos - (256*sizeof(int) + sizeof(int) + sizeof(int) + extLen);
    fseek(f, (256*sizeof(int) + sizeof(int) + sizeof(int) + extLen), SEEK_SET);

    unsigned char* data = malloc(dataSize);
    fread(data, 1, dataSize, f);
    fclose(f);

    int totalBits = dataSize * 8;
    unsigned char* decoded = decode(data, totalBits, root, originalSize);

    char finalPath[512];
    snprintf(finalPath, sizeof(finalPath), "%s%s", outputBase, ext);

    FILE* out = fopen(finalPath, "wb");
    fwrite(decoded, 1, originalSize, out);
    fclose(out);

    free(data);
    free(decoded);

    return 1;
}


