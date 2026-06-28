using namespace std;

#ifndef DCT_H
#define DCT_H

void initDctTables();
void forwardDctDirect(const float input[64], float output[64]);
void inverseDctDirect(const float input[64], float output[64]);
void forwardDctFast(const float input[64], float output[64]);
void inverseDctFast(const float input[64], float output[64]);

#endif
