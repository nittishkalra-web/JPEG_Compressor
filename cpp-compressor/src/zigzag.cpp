#include "zigzag.h"
using namespace std;


const array<int, 64>& zigzagOrder() {
  static const array<int, 64> order = {
    0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
    12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
    35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
    58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
  };
  return order;
}

array<int16_t, 64> zigzagScan(const int16_t block[64]) {
  array<int16_t, 64> out{};
  const auto& order = zigzagOrder();
  for (int i = 0; i < 64; ++i) out[i] = block[order[i]];
  return out;
}
