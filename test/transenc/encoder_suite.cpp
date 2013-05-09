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
#include <protoc/output.hpp>
#include <protoc/output_array.hpp>
#include <protoc/transenc/encoder.hpp>
#include <protoc/transenc/codes.hpp>

using namespace protoc;

template<std::size_t N>
struct test_array : public output_array<protoc::uint8_t, N>
{
};

BOOST_AUTO_TEST_SUITE(transenc_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_null);
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_true);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_false);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x01);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(127)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_32)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-32)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xE0);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_33)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-33)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xDF);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-128)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x80);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x01);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(-1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_two)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x01);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(-1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_two)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_three)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_four)
{
    test_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x01);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_int64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[2], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[5], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[6], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xFF);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_two)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_three)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_four)
{
    test_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_five)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_six)
{
    test_array<6> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_seven)
{
    test_array<7> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_eight)
{
    test_array<8> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x3F);
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xBF);
}

BOOST_AUTO_TEST_CASE(test_float_two)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x40);
}

BOOST_AUTO_TEST_CASE(test_float_minus_two)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xC0);
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[4], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::quiet_NaN()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float32);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0xC0);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_two)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_three)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_four)
{
    test_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_zero)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
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
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x3F);
}

BOOST_AUTO_TEST_CASE(test_double_minus_one)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xBF);
}

BOOST_AUTO_TEST_CASE(test_double_two)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x40);
}

BOOST_AUTO_TEST_CASE(test_double_minus_two)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xC0);
}

BOOST_AUTO_TEST_CASE(test_double_infinity)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xF0);
    BOOST_REQUIRE_EQUAL(buffer[8], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    test_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_float64);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[4], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[5], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[6], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[7], 0xF8);
    BOOST_REQUIRE_EQUAL(buffer[8], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_two)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_three)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_four)
{
    test_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_five)
{
    test_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_six)
{
    test_array<6> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_seven)
{
    test_array<7> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_eight)
{
    test_array<8> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_empty)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_binary_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
}

BOOST_AUTO_TEST_CASE(test_binary_empty_missing_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_empty_missing_two)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_one)
{
    test_array<2+1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back(0x12);
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_binary_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x01);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x12);
}

BOOST_AUTO_TEST_CASE(test_binary_two)
{
    test_array<2+2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back(0x12);
    data.push_back(0x34);
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 4);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_binary_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x02);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x12);
    BOOST_REQUIRE_EQUAL(buffer[3], 0x34);
}

BOOST_AUTO_TEST_CASE(test_binary_missing_one)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back(0x12);
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_missing_two)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back(0x12);
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_missing_three)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back(0x12);
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_string_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    test_array<2+5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 7);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_string_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x05);
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3], 'l');
    BOOST_REQUIRE_EQUAL(buffer[4], 'p');
    BOOST_REQUIRE_EQUAL(buffer[5], 'h');
    BOOST_REQUIRE_EQUAL(buffer[6], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_ab)
{
    test_array<2+2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 4);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_string_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x02);
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3], 'b');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_empty)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_one)
{
    test_array<2+1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_alpha_empty)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_medium_a)
{
    test_array<2+0x80> buffer;
    transenc::encoder encoder(buffer);
    std::string data(0x80, 'a');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2+0x80);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_string_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[2+0x7F], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_larger_a)
{
    test_array<3+0x100> buffer;
    transenc::encoder encoder(buffer);
    std::string data(0x100, 'a');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 3+0x100);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_string_int16);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x01);
    BOOST_REQUIRE_EQUAL(buffer[3], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3+0x7F], 'a');
}

//-----------------------------------------------------------------------------
// Numeric tag
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_tag8_zero)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(protoc::int8_t(0)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xA3);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
}

BOOST_AUTO_TEST_CASE(test_tag8_one)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(protoc::int8_t(1)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xA3);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x01);
}

BOOST_AUTO_TEST_CASE(test_tag8_127)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(protoc::int8_t(127)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xA3);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x7F);
}

BOOST_AUTO_TEST_CASE(test_tag16_128)
{
    test_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(protoc::int16_t(128)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xB3);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x80);
    BOOST_REQUIRE_EQUAL(buffer[2], 0x00);
}

//-----------------------------------------------------------------------------
// Named tag
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_name_empty)
{
    test_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(""), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_name_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x00);
}

BOOST_AUTO_TEST_CASE(test_name_buffer_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_name_buffer_one)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_name_alpha)
{
    test_array<2+5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tag("alpha"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 7);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_name_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0x05);
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3], 'l');
    BOOST_REQUIRE_EQUAL(buffer[4], 'p');
    BOOST_REQUIRE_EQUAL(buffer[5], 'h');
    BOOST_REQUIRE_EQUAL(buffer[6], 'a');
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_begin)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_array_begin);
}

BOOST_AUTO_TEST_CASE(test_array_begin_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_array_end)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_array_end);
}

BOOST_AUTO_TEST_CASE(test_array_end_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_tuple_begin)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_tuple_begin);
}

BOOST_AUTO_TEST_CASE(test_tuple_begin_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_tuple_end)
{
    test_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], transenc::code_tuple_end);
}

BOOST_AUTO_TEST_CASE(test_tuple_end_empty)
{
    test_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
