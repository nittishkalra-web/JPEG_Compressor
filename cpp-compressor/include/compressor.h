#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <cstdint>
#include <string>
#include <vector>
using namespace std;


struct VisualBlock {
  vector<double> originalY;
  vector<double> dct;
  vector<int> quantized;
  vector<int> zigzag;
  vector<string> rle;
};

struct CompressionStats {
  int width = 0;
  int height = 0;
  int quality = 50;
  string filter = "none";
  string mode = "optimized";
  long long originalSizeBytes = 0;
  long long estimatedCompressedBytes = 0;
  double compressionRatio = 0.0;
  double mse = 0.0;
  double psnr = 0.0;
  int blockCount = 0;
  double zeroCoefficientPercent = 0.0;
  int rleSymbolCount = 0;
  long long huffmanBitCount = 0;
  double baselineMs = 0.0;
  double optimizedMs = 0.0;
  double speedup = 0.0;
  VisualBlock visualBlock;
};

CompressionStats compressImage(const string& inputPath, const string& outputPath, int quality, const string& filter, const string& mode);

#endif
