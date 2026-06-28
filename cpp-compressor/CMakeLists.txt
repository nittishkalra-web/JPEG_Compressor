cmake_minimum_required(VERSION 3.15)
project(JpegCompressor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(jpeg_compressor
  src/image_io.cpp
  src/color.cpp
  src/filters.cpp
  src/dct.cpp
  src/quantization.cpp
  src/zigzag.cpp
  src/rle.cpp
  src/huffman.cpp
  src/metrics.cpp
  src/compressor.cpp
  src/json_writer.cpp
  src/main.cpp
)

target_include_directories(jpeg_compressor PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external
)

if(WIN32)
  set_target_properties(jpeg_compressor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
  )
endif()

