#include "color.h"
#include <algorithm>
#include <cmath>
using namespace std;


float clampFloat(float value, float low, float high) {
  return max(low, min(high, value));
}

YCbCrImage rgbToYCbCr(const Image& image) {
  YCbCrImage out;
  out.width = image.width;
  out.height = image.height;
  size_t n = (size_t)image.width * image.height;
  out.y.resize(n);
  out.cb.resize(n);
  out.cr.resize(n);
  for (size_t i = 0; i < n; ++i) {
    float r = image.rgb[i * 3 + 0];
    float g = image.rgb[i * 3 + 1];
    float b = image.rgb[i * 3 + 2];
    out.y[i] = 0.299f * r + 0.587f * g + 0.114f * b;
    out.cb[i] = 128.0f - 0.168736f * r - 0.331264f * g + 0.5f * b;
    out.cr[i] = 128.0f + 0.5f * r - 0.418688f * g - 0.081312f * b;
  }
  return out;
}

Image yCbCrToRgb(const YCbCrImage& image) {
  Image out;
  out.width = image.width;
  out.height = image.height;
  out.rgb.resize((size_t)image.width * image.height * 3);
  for (size_t i = 0; i < (size_t)image.width * image.height; ++i) {
    float y = image.y[i], cb = image.cb[i] - 128.0f, cr = image.cr[i] - 128.0f;
    float r = y + 1.402f * cr;
    float g = y - 0.344136f * cb - 0.714136f * cr;
    float b = y + 1.772f * cb;
    out.rgb[i * 3 + 0] = (uint8_t)lround(clampFloat(r, 0, 255));
    out.rgb[i * 3 + 1] = (uint8_t)lround(clampFloat(g, 0, 255));
    out.rgb[i * 3 + 2] = (uint8_t)lround(clampFloat(b, 0, 255));
  }
  return out;
}
