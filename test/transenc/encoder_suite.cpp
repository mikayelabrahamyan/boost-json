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

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x82');
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x81');
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x80');
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(127)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x7F');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_32)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-32)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xE0');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_33)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-33)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xDF');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-128)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x80');
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xB0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xB0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(-1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xB0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_two)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(-1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[3], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[4], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_two)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_three)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_four)
{
    output_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD0');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[3], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[4], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[5], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[6], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[7], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[8], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_two)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_three)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_four)
{
    output_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_five)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_six)
{
    output_array<6> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_seven)
{
    output_array<7> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_eight)
{
    output_array<8> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x3F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xBF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_two)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x40');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_two)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x7F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float32_t>::infinity()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::quiet_NaN()), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xC2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x7F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_two)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_three)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_four)
{
    output_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_zero)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x3F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xF0');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_minus_one)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xBF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xF0');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_two)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x40');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_minus_two)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_infinity)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x7F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xF0');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xF0');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    output_array<9> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xD2');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x7F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xF8');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[5], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[6], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[7], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_two)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_three)
{
    output_array<3> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_four)
{
    output_array<4> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_five)
{
    output_array<5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_six)
{
    output_array<6> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_seven)
{
    output_array<7> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_eight)
{
    output_array<8> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_empty)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA8');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
}

BOOST_AUTO_TEST_CASE(test_binary_empty_missing_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_empty_missing_two)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_one)
{
    output_array<2+1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back('\x12');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA8');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x01');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x12');
}

BOOST_AUTO_TEST_CASE(test_binary_two)
{
    output_array<2+2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back('\x12');
    data.push_back('\x34');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 4);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA8');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x02');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x12');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x34');
}

BOOST_AUTO_TEST_CASE(test_binary_missing_one)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back('\x12');
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_missing_two)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back('\x12');
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_binary_missing_three)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    std::vector<protoc::int8_t> data;
    data.push_back('\x12');
    BOOST_REQUIRE_EQUAL(encoder.put(data), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA9');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_one)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    output_array<2+5> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 7);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA9');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x05');
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3], 'l');
    BOOST_REQUIRE_EQUAL(buffer[4], 'p');
    BOOST_REQUIRE_EQUAL(buffer[5], 'h');
    BOOST_REQUIRE_EQUAL(buffer[6], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_ab)
{
    output_array<2+2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 4);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xA9');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x02');
    BOOST_REQUIRE_EQUAL(buffer[2], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3], 'b');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_empty)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_one)
{
    output_array<2+1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_alpha_empty)
{
    output_array<2> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_medium_a)
{
    output_array<3+0x80> buffer;
    transenc::encoder encoder(buffer);
    std::string data(0x80, 'a');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 3+0x80);
    BOOST_REQUIRE_EQUAL(buffer[0], '\xB9');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], 'a');
    BOOST_REQUIRE_EQUAL(buffer[3+0x7F], 'a');
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_begin)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x91');
}

BOOST_AUTO_TEST_CASE(test_array_begin_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_array_end)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x99');
}

BOOST_AUTO_TEST_CASE(test_array_end_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_tuple_begin)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x90');
}

BOOST_AUTO_TEST_CASE(test_tuple_begin_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_tuple_end)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x98');
}

BOOST_AUTO_TEST_CASE(test_tuple_end_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_tuple_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
