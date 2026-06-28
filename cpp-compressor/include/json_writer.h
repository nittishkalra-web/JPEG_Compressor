#ifndef JSON_WRITER_H
#define JSON_WRITER_H

#include "compressor.h"
#include <string>
using namespace std;


void writeStatsJson(const string& path, const CompressionStats& stats);

#endif
