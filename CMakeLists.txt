###############################################################################
#
# http://protoc.sourceforge.net/
#
# Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
# MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
# CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
#
###############################################################################

# Debug build
#   cmake -DCMAKE_BUILD_TYPE=Debug .

###############################################################################

cmake_minimum_required(VERSION 2.8)
project(protoc)

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

###############################################################################
# Boost package
###############################################################################

# FIXME: Probably ok to use older version than 1.47
find_package(Boost 1.47.0 COMPONENTS unit_test_framework serialization)
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "Boost not found (or too old)")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

###############################################################################
# protoc package
###############################################################################

include_directories(BEFORE include)
link_directories(BEFORE lib)
set(LIBRARY_OUTPUT_PATH lib)
set(EXTRA_LIBS -lprotoc ${EXTRA_LIBS})

add_library(protoc STATIC
  src/input_range.cpp
  src/msgpack/decoder.cpp
  src/output_vector.cpp
  src/msgpack/decoder.cpp
  src/msgpack/encoder.cpp
  src/transenc/decoder.cpp
  src/transenc/encoder.cpp
  src/ubjson/decoder.cpp
  src/ubjson/encoder.cpp
  src/ubjson/iarchive.cpp
  src/ubjson/oarchive.cpp
)

###############################################################################
# Test
###############################################################################

add_executable(protoctest
  test/runner.cpp
  test/msgpack/decoder_suite.cpp
  test/msgpack/encoder_suite.cpp
  test/transenc/decoder_suite.cpp
  test/transenc/encoder_suite.cpp
  test/ubjson/decoder_suite.cpp
  test/ubjson/encoder_suite.cpp
  test/ubjson/iarchive_suite.cpp
  test/ubjson/oarchive_suite.cpp
)

add_dependencies(protoctest protoc)
set_target_properties(protoctest PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
set_target_properties(protoctest PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib)
target_link_libraries(protoctest ${EXTRA_LIBS})
