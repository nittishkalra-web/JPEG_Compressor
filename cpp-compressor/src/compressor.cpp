#include "compressor.h"
#include "color.h"
#include "dct.h"
#include "filters.h"
#include "huffman.h"
#include "image_io.h"
#include "metrics.h"
#include "quantization.h"
#include "rle.h"
#include "zigzag.h"
#include <algorithm>
#include <chrono>
using namespace std;


struct RunData {
  YCbCrImage reconstructed;
  long long zeroCount = 0;
  long long coeffCount = 0;
  vector<RleSymbol> allSymbols;
  VisualBlock visual;
  double ms = 0.0;
};

static float sampleClamped(const vector<float>& plane, int w, int h, int x, int y) {
  x = max(0, min(w - 1, x));
  y = max(0, min(h - 1, y));
  return plane[(size_t)y * w + x];
}

static void processPlane(const vector<float>& src, vector<float>& dst, int w, int h, int quality, bool optimized, RunData& data, bool captureVisual) {
  float block[64], dctBlock[64], deq[64], recon[64];
  int16_t qBlock[64];
  vector<int16_t> zzVec(64);
  for (int by = 0; by < h; by += 8) {
    for (int bx = 0; bx < w; bx += 8) {
      for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
          block[y * 8 + x] = sampleClamped(src, w, h, bx + x, by + y);

      if (optimized) forwardDctFast(block, dctBlock); else forwardDctDirect(block, dctBlock);
      quantizeBlock(dctBlock, qBlock, quality);
      for (int i = 0; i < 64; ++i) if (qBlock[i] == 0) data.zeroCount++;
      data.coeffCount += 64;

      auto zz = zigzagScan(qBlock);
      for (int i = 0; i < 64; ++i) zzVec[i] = zz[i];
      auto symbols = runLengthEncode(zzVec);
      data.allSymbols.insert(data.allSymbols.end(), symbols.begin(), symbols.end());

      if (captureVisual && data.visual.originalY.empty()) {
        for (int i = 0; i < 64; ++i) {
          data.visual.originalY.push_back(block[i]);
          data.visual.dct.push_back(dctBlock[i]);
          data.visual.quantized.push_back(qBlock[i]);
          data.visual.zigzag.push_back(zz[i]);
        }
        data.visual.rle = rleToStrings(symbols);
      }

      dequantizeBlock(qBlock, deq, quality);
      if (optimized) inverseDctFast(deq, recon); else inverseDctDirect(deq, recon);
      for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
        int px = bx + x, py = by + y;
        if (px < w && py < h) dst[(size_t)py * w + px] = clampFloat(recon[y * 8 + x], 0, 255);
      }
    }
  }
}

static RunData runPipeline(const YCbCrImage& input, int quality, bool optimized) {
  auto start = chrono::high_resolution_clock::now();
  RunData data;
  data.reconstructed = input;
  data.reconstructed.y.assign((size_t)input.width * input.height, 0);
  data.reconstructed.cb.assign((size_t)input.width * input.height, 128);
  data.reconstructed.cr.assign((size_t)input.width * input.height, 128);
  processPlane(input.y, data.reconstructed.y, input.width, input.height, quality, optimized, data, true);
  processPlane(input.cb, data.reconstructed.cb, input.width, input.height, quality, optimized, data, false);
  processPlane(input.cr, data.reconstructed.cr, input.width, input.height, quality, optimized, data, false);
  auto end = chrono::high_resolution_clock::now();
  data.ms = chrono::duration<double, milli>(end - start).count();
  return data;
}

CompressionStats compressImage(const string& inputPath, const string& outputPath, int quality, const string& filter, const string& mode) {
  initDctTables();
  Image original = loadImage(inputPath);
  YCbCrImage ycc = rgbToYCbCr(original);
  applyFilter(ycc, filter);

  bool compare = mode == "compare";
  bool useBaseline = mode == "baseline";
  RunData baseline;
  if (compare || useBaseline) baseline = runPipeline(ycc, quality, false);
  RunData optimized;
  if (compare || !useBaseline) optimized = runPipeline(ycc, quality, true);
  RunData& chosen = useBaseline ? baseline : optimized;

  Image reconstructed = yCbCrToRgb(chosen.reconstructed);
  saveImage(outputPath, reconstructed);

  CompressionStats stats;
  stats.width = original.width;
  stats.height = original.height;
  stats.quality = quality;
  stats.filter = filter;
  stats.mode = mode;
  stats.originalSizeBytes = fileSizeBytes(inputPath);
  stats.huffmanBitCount = estimateHuffmanBits(chosen.allSymbols);
  stats.estimatedCompressedBytes = max<long long>(1, (stats.huffmanBitCount + 7) / 8 + 128);
  stats.compressionRatio = stats.originalSizeBytes ? (double)stats.originalSizeBytes / stats.estimatedCompressedBytes : 0.0;
  stats.mse = meanSquaredError(original, reconstructed);
  stats.psnr = psnrFromMse(stats.mse);
  stats.blockCount = ((original.width + 7) / 8) * ((original.height + 7) / 8) * 3;
  stats.zeroCoefficientPercent = chosen.coeffCount ? 100.0 * (double)chosen.zeroCount / (double)chosen.coeffCount : 0.0;
  stats.rleSymbolCount = (int)chosen.allSymbols.size();
  stats.baselineMs = (compare || useBaseline) ? baseline.ms : 0.0;
  stats.optimizedMs = (compare || !useBaseline) ? optimized.ms : 0.0;
  stats.speedup = (stats.baselineMs > 0.0 && stats.optimizedMs > 0.0) ? stats.baselineMs / stats.optimizedMs : 0.0;
  stats.visualBlock = chosen.visual;
  return stats;
}
