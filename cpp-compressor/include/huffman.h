#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "rle.h"
#include <vector>
using namespace std;


long long estimateHuffmanBits(const vector<RleSymbol>& symbols);

#endif
