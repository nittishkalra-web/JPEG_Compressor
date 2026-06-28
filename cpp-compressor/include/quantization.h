#ifndef QUANTIZATION_H
#define QUANTIZATION_H

#include <cstdint>
using namespace std;


int scaledQuantValue(int index, int quality);
void quantizeBlock(const float input[64], int16_t output[64], int quality);
void dequantizeBlock(const int16_t input[64], float output[64], int quality);

#endif
