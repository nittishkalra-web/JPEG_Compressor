#ifndef METRICS_H
#define METRICS_H

#include "image_io.h"
using namespace std;


double meanSquaredError(const Image& a, const Image& b);
double psnrFromMse(double mse);

#endif
