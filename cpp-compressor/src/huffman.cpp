#include "huffman.h"
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;


long long estimateHuffmanBits(const vector<RleSymbol>& symbols) {
  if (symbols.empty()) return 0;
  unordered_map<string, long long> freq;
  for (const auto& s : symbols) freq[rleSymbolKey(s)]++;
  if (freq.size() == 1) return (long long)symbols.size();
  priority_queue<long long, vector<long long>, greater<long long>> pq;
  for (const auto& kv : freq) pq.push(kv.second);
  long long total = 0;
  while (pq.size() > 1) {
    long long a = pq.top(); pq.pop();
    long long b = pq.top(); pq.pop();
    total += a + b;
    pq.push(a + b);
  }
  return total;
}
