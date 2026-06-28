#ifndef RLE_H
#define RLE_H

#include <cstdint>
#include <string>
#include <vector>
using namespace std;


struct RleSymbol {
  int zeros = 0;
  int value = 0;
  bool eob = false;
};

vector<RleSymbol> runLengthEncode(const vector<int16_t>& values);
string rleSymbolKey(const RleSymbol& symbol);
vector<string> rleToStrings(const vector<RleSymbol>& symbols);

#endif
