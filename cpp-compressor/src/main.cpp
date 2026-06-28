#include "compressor.h"
#include "json_writer.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;


static string argValue(int argc, char** argv, const string& key, const string& fallback) {
  for (int i = 4; i + 1 < argc; ++i) {
    if (argv[i] == key) return argv[i + 1];
  }
  return fallback;
}

static bool contains(const vector<string>& values, const string& value) {
  return find(values.begin(), values.end(), value) != values.end();
}

int main(int argc, char** argv) {
  if (argc < 4) {
    cerr << "Usage: jpeg_compressor input_image output_image stats_json --quality 50 --filter warm --mode optimized\n";
    return 1;
  }
  try {
    string input = argv[1];
    string output = argv[2];
    string json = argv[3];
    int quality = stoi(argValue(argc, argv, "--quality", "50"));
    quality = max(1, min(100, quality));
    string filter = argValue(argc, argv, "--filter", "none");
    string mode = argValue(argc, argv, "--mode", "optimized");
    const vector<string> filters = {"none", "grayscale", "warm", "cool", "vintage", "contrast", "saturation"};
    const vector<string> modes = {"baseline", "optimized", "compare"};
    if (!contains(filters, filter)) throw runtime_error("Unsupported filter: " + filter);
    if (!contains(modes, mode)) throw runtime_error("Unsupported mode: " + mode);
    CompressionStats stats = compressImage(input, output, quality, filter, mode);
    writeStatsJson(json, stats);
    return 0;
  } catch (const exception& ex) {
    cerr << ex.what() << "\n";
    return 2;
  }
}
