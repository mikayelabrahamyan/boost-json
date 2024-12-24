cmake_minimum_required(VERSION 3.7)
project(protoc)

find_package(Boost 1.49.0 COMPONENTS serialization)
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "Boost not found (or too old)")
endif()

add_library(protoc STATIC
  src/json/decoder.cpp
  src/json/encoder.cpp
  src/msgpack/decoder.cpp
  src/msgpack/encoder.cpp
  src/msgpack/reader.cpp
  src/msgpack/writer.cpp
  src/transenc/decoder.cpp
  src/transenc/encoder.cpp
  src/ubjson/decoder.cpp
  src/ubjson/encoder.cpp
  src/ubjson/iarchive.cpp
  src/ubjson/oarchive.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})