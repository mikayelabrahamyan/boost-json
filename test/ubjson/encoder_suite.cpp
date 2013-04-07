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
#include <protoc/output_array.hpp>
#include <protoc/ubjson/encoder.hpp>

using namespace protoc;

template<std::size_t N>
struct test_array : public output_array<char, N>
{
};

BOOST_AUTO_TEST_SUITE(ubjson_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_true)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'T');
}

BOOST_AUTO_TEST_CASE(test_false)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'F');
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(127)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x7F');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-127)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x81');
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-128)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], 'B');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x80');
}

BOOST_AUTO_TEST_CASE(test_int8_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int8_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], 'i');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], 'i');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(-1)), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], 'i');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int16_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int16_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'I');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'I');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x01');
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(-1)), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'I');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[3], '\xFF');
    BOOST_REQUIRE_EQUAL(buffer[4], '\xFF');
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_three)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int32_buffer_four)
{
    test_array<4> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int32_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'L');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'L');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'L');
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
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_three)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_four)
{
    test_array<4> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_five)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_six)
{
    test_array<6> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_seven)
{
    test_array<7> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_int64_buffer_eight)
{
    test_array<8> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'd');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'd');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x3F');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'd');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xBF');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_two)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'd');
    BOOST_REQUIRE_EQUAL(buffer[1], '\x40');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_minus_two)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0f), 5);
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 'd');
    BOOST_REQUIRE_EQUAL(buffer[1], '\xC0');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[4], '\x00');
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float32_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float32_t>::quiet_NaN()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_float_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_three)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_float_buffer_four)
{
    test_array<4> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0f), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_zero)
{
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'D');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'D');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'D');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'D');
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
    test_array<9> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-2.0), 9);
    BOOST_REQUIRE_EQUAL(buffer.size(), 9);
    BOOST_REQUIRE_EQUAL(buffer[0], 'D');
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
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_minus_infinity)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_nan)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 'Z');
}

BOOST_AUTO_TEST_CASE(test_double_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_three)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_four)
{
    test_array<4> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_five)
{
    test_array<5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_six)
{
    test_array<6> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_seven)
{
    test_array<7> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_double_buffer_eight)
{
    test_array<8> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 3);
    BOOST_REQUIRE_EQUAL(buffer.size(), 3);
    BOOST_REQUIRE_EQUAL(buffer[0], 's');
    BOOST_REQUIRE_EQUAL(buffer[1], 'B');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_one)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_two)
{
    test_array<2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    test_array<3+5> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 8);
    BOOST_REQUIRE_EQUAL(buffer[0], 's');
    BOOST_REQUIRE_EQUAL(buffer[1], 'B');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x05');
    BOOST_REQUIRE_EQUAL(buffer[3], 'a');
    BOOST_REQUIRE_EQUAL(buffer[4], 'l');
    BOOST_REQUIRE_EQUAL(buffer[5], 'p');
    BOOST_REQUIRE_EQUAL(buffer[6], 'h');
    BOOST_REQUIRE_EQUAL(buffer[7], 'a');
}

BOOST_AUTO_TEST_CASE(test_string_ab)
{
    test_array<3+2> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 5);
    BOOST_REQUIRE_EQUAL(buffer[0], 's');
    BOOST_REQUIRE_EQUAL(buffer[1], 'B');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x02');
    BOOST_REQUIRE_EQUAL(buffer[3], 'a');
    BOOST_REQUIRE_EQUAL(buffer[4], 'b');
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_empty)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_ab_one)
{
    test_array<3+1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("ab"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_buffer_alpha_empty)
{
    test_array<3> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_medium_a)
{
    test_array<4+0x80> buffer;
    ubjson::encoder encoder(buffer);
    std::string data(0x80, 'a');
    BOOST_REQUIRE_EQUAL(encoder.put(data), buffer.capacity());
    BOOST_REQUIRE_EQUAL(buffer.size(), 4+0x80);
    BOOST_REQUIRE_EQUAL(buffer[0], 's');
    BOOST_REQUIRE_EQUAL(buffer[1], 'i');
    BOOST_REQUIRE_EQUAL(buffer[2], '\x00');
    BOOST_REQUIRE_EQUAL(buffer[3], '\x80');
    BOOST_REQUIRE_EQUAL(buffer[4], 'a');
    BOOST_REQUIRE_EQUAL(buffer[4+0x7F], 'a');
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_begin)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '[');
}

BOOST_AUTO_TEST_CASE(test_array_begin_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_array_end)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], ']');
}

BOOST_AUTO_TEST_CASE(test_array_end_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_object_begin)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '{');
}

BOOST_AUTO_TEST_CASE(test_object_begin_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_object_end)
{
    test_array<1> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '}');
}

BOOST_AUTO_TEST_CASE(test_object_end_empty)
{
    test_array<0> buffer;
    ubjson::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
