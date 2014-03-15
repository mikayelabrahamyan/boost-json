///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
// MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
// CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <limits>
#include <cmath> // std::abs
#include <protoc/output.hpp>
#include <protoc/output_array.hpp>
#include <protoc/output_vector.hpp>
#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/detail/encoder.hpp>

namespace format = protoc::msgpack::detail;

template<std::size_t N>
struct test_array : public protoc::output_array<unsigned char, N>
{
};

struct test_vector : public protoc::output_vector<unsigned char>
{
};

BOOST_AUTO_TEST_SUITE(msgpack_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_null);
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_true);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_false);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_vector_null)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_null);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x01);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int8_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::int8_t>::max()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_int8_fixmin)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-32)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xE0);
}

BOOST_AUTO_TEST_CASE(test_int8_below_fixmin)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-33)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xDF);
}

BOOST_AUTO_TEST_CASE(test_uint8_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put((unsigned int)std::numeric_limits<protoc::uint8_t>::max()), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_uint8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int16_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::int16_t>::max()), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_int16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_uint16_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put((unsigned int)std::numeric_limits<protoc::uint16_t>::max()), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_uint16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int32_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::int32_t>::max()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_int32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_uint32_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put((unsigned int)std::numeric_limits<protoc::uint32_t>::max()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_uint32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int64_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::int64_t>::max()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_int64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[5], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[6], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_uint64_max)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put((protoc::uint64_t)std::numeric_limits<protoc::uint64_t>::max()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_uint64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[5], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[6], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xFF);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x3F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xBF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    test_array<5> buffer;
    format::encoder encoder(buffer);
    // std::abs() is a workaround for clang 3.2, which sets the negative sign on NaN
    BOOST_REQUIRE_EQUAL(encoder.put(std::abs(std::numeric_limits<protoc::float32_t>::quiet_NaN())), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xC0);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_zero)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x3F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_minus_one)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xBF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_infinity)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    test_array<9> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::abs(std::numeric_limits<protoc::float64_t>::quiet_NaN())), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xF8);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_fixstr_0);
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    test_array<1+5> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 6);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_fixstr_5);
    BOOST_REQUIRE_EQUAL(buffer[1], 'a');
    BOOST_REQUIRE_EQUAL(buffer[2], 'l');
    BOOST_REQUIRE_EQUAL(buffer[3], 'p');
    BOOST_REQUIRE_EQUAL(buffer[4], 'h');
    BOOST_REQUIRE_EQUAL(buffer[5], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_fixstr_max)
{
    test_array<1+31> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ABCDEFGHIJKLMNOPQRSTUVWXYZ01234"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 32);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_fixstr_31);
    BOOST_REQUIRE_EQUAL(buffer[1], 'A');
    BOOST_REQUIRE_EQUAL(buffer[31], '4');
}

BOOST_AUTO_TEST_CASE(test_string_above_fixstr_max)
{
    test_array<2+32> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ABCDEFGHIJKLMNOPQRSTUVWXYZ012345"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 34);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_str8);
    BOOST_REQUIRE_EQUAL(buffer[1], 32);
    BOOST_REQUIRE_EQUAL(buffer[2], 'A');
    BOOST_REQUIRE_EQUAL(buffer[33], '5');
}

BOOST_AUTO_TEST_SUITE_END()
