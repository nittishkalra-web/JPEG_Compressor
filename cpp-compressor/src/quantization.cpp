#include "quantization.h"
#include <algorithm>
#include <cmath>
using namespace std;


static const int Q[64] = {
  16,11,10,16,24,40,51,61, 12,12,14,19,26,58,60,55,
  14,13,16,24,40,57,69,56, 14,17,22,29,51,87,80,62,
  18,22,37,56,68,109,103,77, 24,35,55,64,81,104,113,92,
  49,64,78,87,103,121,120,101, 72,92,95,98,112,100,103,99
};

int scaledQuantValue(int index, int quality) {
  quality = max(1, min(100, quality));
  int scale = quality < 50 ? 5000 / quality : 200 - quality * 2;
  int value = (Q[index] * scale + 50) / 100;
  return max(1, min(255, value));
}

void quantizeBlock(const float input[64], int16_t output[64], int quality) {
  for (int i = 0; i < 64; ++i) output[i] = (int16_t)lround(input[i] / scaledQuantValue(i, quality));
}

void dequantizeBlock(const int16_t input[64], float output[64], int quality) {
  for (int i = 0; i < 64; ++i) output[i] = (float)input[i] * scaledQuantValue(i, quality);
}
