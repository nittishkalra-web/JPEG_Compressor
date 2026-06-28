#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <cstdint>
#include <string>
#include <vector>
using namespace std;


struct Image {
  int width = 0;
  int height = 0;
  vector<uint8_t> rgb;
};

Image loadImage(const string& path);
void saveImage(const string& path, const Image& image);
long long fileSizeBytes(const string& path);

#endif
