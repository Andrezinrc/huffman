#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

/*
 * Conta a frequência de cada byte.
 */
int* countFrequency(const unsigned char* text, size_t length);

#endif
