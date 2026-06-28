#ifndef ZIGZAG_H
#define ZIGZAG_H

#include <array>
#include <cstdint>
using namespace std;


const array<int, 64>& zigzagOrder();
array<int16_t, 64> zigzagScan(const int16_t block[64]);

#endif
