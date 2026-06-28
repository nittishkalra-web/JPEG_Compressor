#include "json_writer.h"
#include <cmath>
#include <fstream>
#include <iomanip>
using namespace std;


static void writeNumberArray(ofstream& out, const vector<double>& values) {
  out << "[";
  for (size_t i = 0; i < values.size(); ++i) {
    if (i) out << ",";
    out << fixed << setprecision(2) << values[i];
  }
  out << "]";
}

static void writeIntArray(ofstream& out, const vector<int>& values) {
  out << "[";
  for (size_t i = 0; i < values.size(); ++i) {
    if (i) out << ",";
    out << values[i];
  }
  out << "]";
}

static void writeStringArray(ofstream& out, const vector<string>& values) {
  out << "[";
  for (size_t i = 0; i < values.size(); ++i) {
    if (i) out << ",";
    out << "\"" << values[i] << "\"";
  }
  out << "]";
}

void writeStatsJson(const string& path, const CompressionStats& s) {
  ofstream out(path);
  out << fixed << setprecision(2);
  out << "{\n";
  out << "  \"width\": " << s.width << ",\n";
  out << "  \"height\": " << s.height << ",\n";
  out << "  \"quality\": " << s.quality << ",\n";
  out << "  \"filter\": \"" << s.filter << "\",\n";
  out << "  \"mode\": \"" << s.mode << "\",\n";
  out << "  \"originalSizeBytes\": " << s.originalSizeBytes << ",\n";
  out << "  \"estimatedCompressedBytes\": " << s.estimatedCompressedBytes << ",\n";
  out << "  \"compressionRatio\": " << s.compressionRatio << ",\n";
  out << "  \"mse\": " << s.mse << ",\n";
  out << "  \"psnr\": " << s.psnr << ",\n";
  out << "  \"blockCount\": " << s.blockCount << ",\n";
  out << "  \"zeroCoefficientPercent\": " << s.zeroCoefficientPercent << ",\n";
  out << "  \"rleSymbolCount\": " << s.rleSymbolCount << ",\n";
  out << "  \"huffmanBitCount\": " << s.huffmanBitCount << ",\n";
  out << "  \"baselineMs\": " << s.baselineMs << ",\n";
  out << "  \"optimizedMs\": " << s.optimizedMs << ",\n";
  out << "  \"speedup\": " << s.speedup << ",\n";
  out << "  \"visualBlock\": {\n";
  out << "    \"originalY\": "; writeNumberArray(out, s.visualBlock.originalY); out << ",\n";
  out << "    \"dct\": "; writeNumberArray(out, s.visualBlock.dct); out << ",\n";
  out << "    \"quantized\": "; writeIntArray(out, s.visualBlock.quantized); out << ",\n";
  out << "    \"zigzag\": "; writeIntArray(out, s.visualBlock.zigzag); out << ",\n";
  out << "    \"rle\": "; writeStringArray(out, s.visualBlock.rle); out << "\n";
  out << "  }\n";
  out << "}\n";
}
