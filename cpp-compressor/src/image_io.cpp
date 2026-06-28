#include "image_io.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>
#include <stdexcept>
using namespace std;


Image loadImage(const string& path) {
  int w = 0, h = 0, comp = 0;
  unsigned char* data = stbi_load(path.c_str(), &w, &h, &comp, 3);
  if (!data) throw runtime_error("Could not load image: " + path);
  Image image;
  image.width = w;
  image.height = h;
  image.rgb.assign(data, data + (size_t)w * h * 3);
  stbi_image_free(data);
  return image;
}

void saveImage(const string& path, const Image& image) {
  if (!stbi_write_png(path.c_str(), image.width, image.height, 3, image.rgb.data(), image.width * 3)) {
    throw runtime_error("Could not save image: " + path);
  }
}

long long fileSizeBytes(const string& path) {
  ifstream file(path, ios::binary | ios::ate);
  if (!file) return 0;
  return static_cast<long long>(file.tellg());
}
