#include "dct.h"
#include <cmath>
using namespace std;


static float cosTable[8][8];
static bool ready = false;

void initDctTables() {
  const double pi = acos(-1.0);
  for (int u = 0; u < 8; ++u)
    for (int x = 0; x < 8; ++x)
      cosTable[u][x] = (float)cos(((2 * x + 1) * u * pi) / 16.0);
  ready = true;
}

static float alpha(int i) { return i == 0 ? 0.70710678118f : 1.0f; }

void forwardDctDirect(const float input[64], float output[64]) {
  if (!ready) initDctTables();
  for (int v = 0; v < 8; ++v) for (int u = 0; u < 8; ++u) {
    float sum = 0;
    for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x)
      sum += (input[y * 8 + x] - 128.0f) * cosTable[u][x] * cosTable[v][y];
    output[v * 8 + u] = 0.25f * alpha(u) * alpha(v) * sum;
  }
}

void inverseDctDirect(const float input[64], float output[64]) {
  if (!ready) initDctTables();
  for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
    float sum = 0;
    for (int v = 0; v < 8; ++v) for (int u = 0; u < 8; ++u)
      sum += alpha(u) * alpha(v) * input[v * 8 + u] * cosTable[u][x] * cosTable[v][y];
    output[y * 8 + x] = 0.25f * sum + 128.0f;
  }
}

void forwardDctFast(const float input[64], float output[64]) { forwardDctDirect(input, output); }
void inverseDctFast(const float input[64], float output[64]) { inverseDctDirect(input, output); }
