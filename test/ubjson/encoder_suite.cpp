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
#include <protoc/ubjson/encoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(ubjson_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_true)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'T');
}

BOOST_AUTO_TEST_CASE(test_false)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'F');
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(127)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\x7F');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-127)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\x81');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-128)), 2);
    BOOST_REQUIRE_EQUAL(output[0], 'B');
    BOOST_REQUIRE_EQUAL(output[1], '\x80');
}

BOOST_AUTO_TEST_CASE(test_int8_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int8_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 3);
    BOOST_REQUIRE_EQUAL(output[0], 'i');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(1)), 3);
    BOOST_REQUIRE_EQUAL(output[0], 'i');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(-1)), 3);
    BOOST_REQUIRE_EQUAL(output[0], 'i');
    BOOST_REQUIRE_EQUAL(output[1], '\xFF');
    BOOST_REQUIRE_EQUAL(output[2], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'I');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(1)), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'I');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(-1)), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'I');
    BOOST_REQUIRE_EQUAL(output[1], '\xFF');
    BOOST_REQUIRE_EQUAL(output[2], '\xFF');
    BOOST_REQUIRE_EQUAL(output[3], '\xFF');
    BOOST_REQUIRE_EQUAL(output[4], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_three)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_four)
{
    char output[4];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'L');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'L');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'L');
    BOOST_REQUIRE_EQUAL(output[1], '\xFF');
    BOOST_REQUIRE_EQUAL(output[2], '\xFF');
    BOOST_REQUIRE_EQUAL(output[3], '\xFF');
    BOOST_REQUIRE_EQUAL(output[4], '\xFF');
    BOOST_REQUIRE_EQUAL(output[5], '\xFF');
    BOOST_REQUIRE_EQUAL(output[6], '\xFF');
    BOOST_REQUIRE_EQUAL(output[7], '\xFF');
    BOOST_REQUIRE_EQUAL(output[8], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_three)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_four)
{
    char output[4];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_five)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_six)
{
    char output[6];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_seven)
{
    char output[7];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_eight)
{
    char output[8];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'd');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(1.0f), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'd');
    BOOST_REQUIRE_EQUAL(output[1], '\x3F');
    BOOST_REQUIRE_EQUAL(output[2], '\x80');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0f), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'd');
    BOOST_REQUIRE_EQUAL(output[1], '\xBF');
    BOOST_REQUIRE_EQUAL(output[2], '\x80');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_two)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(2.0f), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'd');
    BOOST_REQUIRE_EQUAL(output[1], '\x40');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_two)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0f), 5);
    BOOST_REQUIRE_EQUAL(output[0], 'd');
    BOOST_REQUIRE_EQUAL(output[1], '\xC0');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float32_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::quiet_NaN()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_three)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_four)
{
    char output[4];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
}

BOOST_AUTO_TEST_CASE(test_double_zero)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'D');
    BOOST_REQUIRE_EQUAL(output[1], '\x00');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'D');
    BOOST_REQUIRE_EQUAL(output[1], '\x3F');
    BOOST_REQUIRE_EQUAL(output[2], '\xF0');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_minus_one)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'D');
    BOOST_REQUIRE_EQUAL(output[1], '\xBF');
    BOOST_REQUIRE_EQUAL(output[2], '\xF0');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_two)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(2.0), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'D');
    BOOST_REQUIRE_EQUAL(output[1], '\x40');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_minus_two)
{
    char output[9];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0), 9);
    BOOST_REQUIRE_EQUAL(output[0], 'D');
    BOOST_REQUIRE_EQUAL(output[1], '\xC0');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x00');
    BOOST_REQUIRE_EQUAL(output[4], '\x00');
    BOOST_REQUIRE_EQUAL(output[5], '\x00');
    BOOST_REQUIRE_EQUAL(output[6], '\x00');
    BOOST_REQUIRE_EQUAL(output[7], '\x00');
    BOOST_REQUIRE_EQUAL(output[8], '\x00');
}

BOOST_AUTO_TEST_CASE(test_double_infinity)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 1);
    BOOST_REQUIRE_EQUAL(output[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_three)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_four)
{
    char output[4];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_five)
{
    char output[5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_six)
{
    char output[6];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_seven)
{
    char output[7];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_eight)
{
    char output[8];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(""), 3);
    BOOST_REQUIRE_EQUAL(output[0], 's');
    BOOST_REQUIRE_EQUAL(output[1], 'B');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_empty)
{
    char output[0];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_one)
{
    char output[1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_two)
{
    char output[2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    char output[3+5];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), sizeof(output));
    BOOST_REQUIRE_EQUAL(output[0], 's');
    BOOST_REQUIRE_EQUAL(output[1], 'B');
    BOOST_REQUIRE_EQUAL(output[2], '\x05');
    BOOST_REQUIRE_EQUAL(output[3], 'a');
    BOOST_REQUIRE_EQUAL(output[4], 'l');
    BOOST_REQUIRE_EQUAL(output[5], 'p');
    BOOST_REQUIRE_EQUAL(output[6], 'h');
    BOOST_REQUIRE_EQUAL(output[7], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_ab)
{
    char output[3+2];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), sizeof(output));
    BOOST_REQUIRE_EQUAL(output[0], 's');
    BOOST_REQUIRE_EQUAL(output[1], 'B');
    BOOST_REQUIRE_EQUAL(output[2], '\x02');
    BOOST_REQUIRE_EQUAL(output[3], 'a');
    BOOST_REQUIRE_EQUAL(output[4], 'b');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_empty)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_one)
{
    char output[3+1];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_alpha_empty)
{
    char output[3];
    ubjson::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), 0);
}

BOOST_AUTO_TEST_CASE(test_string_medium_a)
{
    char output[4+0x80];
    ubjson::encoder encoder(output, output + sizeof(output));
    std::string data(0x80, 'a');
    BOOST_REQUIRE_EQUAL(encoder.put(data), sizeof(output));
    BOOST_REQUIRE_EQUAL(output[0], 's');
    BOOST_REQUIRE_EQUAL(output[1], 'i');
    BOOST_REQUIRE_EQUAL(output[2], '\x00');
    BOOST_REQUIRE_EQUAL(output[3], '\x80');
    BOOST_REQUIRE_EQUAL(output[4], 'a');
    BOOST_REQUIRE_EQUAL(output[4+0x7F], 'a');
}

BOOST_AUTO_TEST_SUITE_END()
