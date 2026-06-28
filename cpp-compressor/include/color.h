#ifndef COLOR_H
#define COLOR_H

#include "image_io.h"
#include <vector>
using namespace std;


struct YCbCrImage {
  int width = 0;
  int height = 0;
  vector<float> y;
  vector<float> cb;
  vector<float> cr;
};

YCbCrImage rgbToYCbCr(const Image& image);
Image yCbCrToRgb(const YCbCrImage& image);
float clampFloat(float value, float low, float high);

#endif
