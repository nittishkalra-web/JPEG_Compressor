#include "filters.h"
using namespace std;


void applyFilter(YCbCrImage& image, const string& filter) {
  size_t n = image.y.size();
  for (size_t i = 0; i < n; ++i) {
    if (filter == "grayscale") {
      image.cb[i] = 128.0f; image.cr[i] = 128.0f;
    } else if (filter == "warm") {
      image.cb[i] = clampFloat(image.cb[i] - 10.0f, 0, 255);
      image.cr[i] = clampFloat(image.cr[i] + 15.0f, 0, 255);
    } else if (filter == "cool") {
      image.cb[i] = clampFloat(image.cb[i] + 20.0f, 0, 255);
      image.cr[i] = clampFloat(image.cr[i] - 5.0f, 0, 255);
    } else if (filter == "vintage") {
      image.y[i] = clampFloat(image.y[i] + 8.0f, 0, 255);
      image.cb[i] = clampFloat(128.0f + (image.cb[i] - 128.0f) * 0.6f, 0, 255);
      image.cr[i] = clampFloat(128.0f + (image.cr[i] - 128.0f) * 0.6f, 0, 255);
    } else if (filter == "contrast") {
      image.y[i] = clampFloat(128.0f + (image.y[i] - 128.0f) * 1.25f, 0, 255);
    } else if (filter == "saturation") {
      image.cb[i] = clampFloat(128.0f + (image.cb[i] - 128.0f) * 1.3f, 0, 255);
      image.cr[i] = clampFloat(128.0f + (image.cr[i] - 128.0f) * 1.3f, 0, 255);
    }
  }
}
