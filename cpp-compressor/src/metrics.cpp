#include "metrics.h"
#include <cmath>
using namespace std;


double meanSquaredError(const Image& a, const Image& b) {
  if (a.width != b.width || a.height != b.height || a.rgb.size() != b.rgb.size()) return 0.0;
  double sum = 0.0;
  for (size_t i = 0; i < a.rgb.size(); ++i) {
    double d = (double)a.rgb[i] - (double)b.rgb[i];
    sum += d * d;
  }
  return sum / (double)a.rgb.size();
}

double psnrFromMse(double mse) {
  if (mse <= 0.0) return 99.0;
  return 10.0 * log10((255.0 * 255.0) / mse);
}
