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

#include <algorithm> // std::fill_n
#include <protoc/transenc/decoder.hpp>
#include <protoc/transenc/codes.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const protoc::uint8_t input[] = {};
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const protoc::uint8_t input[] = { transenc::code_false };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_false);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const protoc::uint8_t input[] = { transenc::code_true };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_true);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    const protoc::uint8_t input[] = { transenc::code_null };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_small_zero)
{
    const protoc::uint8_t input[] = { 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_one)
{
    const protoc::uint8_t input[] = { 0x01 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_127)
{
    const protoc::uint8_t input[] = { 0x7F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_one)
{
    const protoc::uint8_t input[] = { 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_32)
{
    const protoc::uint8_t input[] = { 0xE0 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -32);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_int8 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x00);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x01 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x01);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x7F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x80 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0x81 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_int8, 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    const protoc::uint8_t input[] = { transenc::code_int16 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0x01, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_int16, 0xFF, 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    const protoc::uint8_t input[] = { transenc::code_int32 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x01, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0xFF, 0xFF, 0xFF, 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x02, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0xFE, 0xFF, 0xFF, 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0xFF, 0xFF, 0xFF, 0x7F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    const protoc::uint8_t input[] = { transenc::code_int32, 0x01, 0x00, 0x00, 0x80 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    const protoc::uint8_t input[] = { transenc::code_int64 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    const protoc::uint8_t input[] = { transenc::code_int64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x80, 0x3F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x80, 0xBF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x00, 0x40 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x00, 0xC0 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_two)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_three)
{
    const protoc::uint8_t input[] = { transenc::code_float32, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_four)
{
    const protoc::uint8_t input[] = { transenc::code_float32 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xBF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_two)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_three)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_four)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_five)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_six)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_seven)
{
    const protoc::uint8_t input[] = { transenc::code_float64, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_eight)
{
    const protoc::uint8_t input[] = { transenc::code_float64 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}


//-----------------------------------------------------------------------------
// Tags
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_tag8_missing_one)
{
    const protoc::uint8_t input[] = { transenc::code_tag8 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_tag8_zero)
{
    const protoc::uint8_t input[] = { transenc::code_tag8, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_tag8);
    BOOST_REQUIRE_EQUAL(decoder.get_tag8(), 0x00);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_tag8_one)
{
    const protoc::uint8_t input[] = { transenc::code_tag8, 0x01 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_tag8);
    BOOST_REQUIRE_EQUAL(decoder.get_tag8(), 0x01);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_tag16_one)
{
    const protoc::uint8_t input[] = { transenc::code_tag16, 0x01, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_tag16);
    BOOST_REQUIRE_EQUAL(decoder.get_tag16(), 0x0001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_tag32_one)
{
    const protoc::uint8_t input[] = { transenc::code_tag32, 0x01, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_tag32);
    BOOST_REQUIRE_EQUAL(decoder.get_tag32(), 0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_tag64_one)
{
    const protoc::uint8_t input[] = { transenc::code_tag64, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_tag64);
    BOOST_REQUIRE_EQUAL(decoder.get_tag64(), 0x0000000000000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Binary data
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_int8_empty)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int8, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int16_empty)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int16, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int32_empty)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int32, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int64_empty)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_one)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int8, 0x01, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "\x12");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int16_one)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int16, 0x01, 0x00, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "\x12");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_two)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int8, 0x02, 0x12, 0x34 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    BOOST_REQUIRE_EQUAL(decoder.get_binary(), "\x12\x34");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_128)
{
    protoc::uint8_t input[2 + 0x80] = { transenc::code_binary_int8, 0x80 };
    std::fill_n(&input[2], sizeof(input) - 2, 0x12);
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_binary);
    std::string binary = decoder.get_binary();
    BOOST_REQUIRE_EQUAL(binary.size(), 0x80);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_missing_length)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int8 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_missing_content)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int8, 0x01 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int64_invalid_length)
{
    const protoc::uint8_t input[] = { transenc::code_binary_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_error);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_int8_empty)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int16_empty)
{
    const protoc::uint8_t input[] = { transenc::code_string_int16, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int32_empty)
{
    const protoc::uint8_t input[] = { transenc::code_string_int32, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int64_empty)
{
    const protoc::uint8_t input[] = { transenc::code_string_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_one)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x01, 'A' };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_two)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8, 0x02, 'A', 'B' };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_missing_length_indicator)
{
    const protoc::uint8_t input[] = { transenc::code_string_int8 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_invalid_length)
{
    const protoc::uint8_t input[] = { transenc::code_string_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_error);
}

//-----------------------------------------------------------------------------
// Names
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_name_int8_empty)
{
    const protoc::uint8_t input[] = { transenc::code_name_int8, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_name);
    BOOST_REQUIRE_EQUAL(decoder.get_name(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int16_empty)
{
    const protoc::uint8_t input[] = { transenc::code_name_int16, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_name);
    BOOST_REQUIRE_EQUAL(decoder.get_name(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int32_empty)
{
    const protoc::uint8_t input[] = { transenc::code_name_int32, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_name);
    BOOST_REQUIRE_EQUAL(decoder.get_name(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int64_empty)
{
    const protoc::uint8_t input[] = { transenc::code_name_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_name);
    BOOST_REQUIRE_EQUAL(decoder.get_name(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int8_two)
{
    const protoc::uint8_t input[] = { transenc::code_name_int8, 0x02, 'A', 'B' };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_name);
    BOOST_REQUIRE_EQUAL(decoder.get_name(), "AB");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int8_missing_length_indicator)
{
    const protoc::uint8_t input[] = { transenc::code_name_int8 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_name_int8_invalid_length)
{
    const protoc::uint8_t input[] = { transenc::code_name_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_error);
}

//-----------------------------------------------------------------------------
// Unknowns
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_unknown_none)
{
    const protoc::uint8_t input[] = { 0x8F };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8)
{
    const protoc::uint8_t input[] = { 0xA7, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_missing_one)
{
    const protoc::uint8_t input[] = { 0xA7 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_empty)
{
    const protoc::uint8_t input[] = { 0xAF, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_one)
{
    const protoc::uint8_t input[] = { 0xAF, 0x01, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_missing_one)
{
    const protoc::uint8_t input[] = { 0xAF };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16)
{
    const protoc::uint8_t input[] = { 0xB7, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_missing_one)
{
    const protoc::uint8_t input[] = { 0xB7, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_empty)
{
    const protoc::uint8_t input[] = { 0xBF, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_one)
{
    const protoc::uint8_t input[] = { 0xBF, 0x01, 0x00, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_missing_one)
{
    const protoc::uint8_t input[] = { 0xBF, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32)
{
    const protoc::uint8_t input[] = { 0xC7, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_missing_one)
{
    const protoc::uint8_t input[] = { 0xC7, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_empty)
{
    const protoc::uint8_t input[] = { 0xCF, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_one)
{
    const protoc::uint8_t input[] = { 0xCF, 0x01, 0x00, 0x00, 0x00, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_missing_one)
{
    const protoc::uint8_t input[] = { 0xCF, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64)
{
    const protoc::uint8_t input[] = { 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_missing_one)
{
    const protoc::uint8_t input[] = { 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_empty)
{
    const protoc::uint8_t input[] = { 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_one)
{
    const protoc::uint8_t input[] = { 0xDF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_missing_one)
{
    const protoc::uint8_t input[] = { 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    transenc::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), transenc::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
