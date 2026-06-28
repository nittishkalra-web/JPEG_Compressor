#include "rle.h"
using namespace std;


vector<RleSymbol> runLengthEncode(const vector<int16_t>& values) {
  vector<RleSymbol> out;
  int zeros = 0;
  for (int v : values) {
    if (v == 0) {
      ++zeros;
    } else {
      while (zeros > 15) {
        out.push_back({15, 0, false});
        zeros -= 16;
      }
      out.push_back({zeros, v, false});
      zeros = 0;
    }
  }
  out.push_back({0, 0, true});
  return out;
}

string rleSymbolKey(const RleSymbol& symbol) {
  if (symbol.eob) return "EOB";
  return "Z" + to_string(symbol.zeros) + ":" + to_string(symbol.value);
}

vector<string> rleToStrings(const vector<RleSymbol>& symbols) {
  vector<string> out;
  for (const auto& s : symbols) {
    if (s.eob) out.push_back("EOB");
    else if (s.zeros > 0) out.push_back("Z" + to_string(s.zeros) + "," + to_string(s.value));
    else out.push_back(to_string(s.value));
  }
  return out;
}
