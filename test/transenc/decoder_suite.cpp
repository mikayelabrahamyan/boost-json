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

#include <protoc/transenc/decoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "\x80";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_false);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "\x81";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_true);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    const char input[] = "\x82";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_small_zero)
{
    const char input[] = "\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_one)
{
    const char input[] = "\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_127)
{
    const char input[] = "\x7F";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_one)
{
    const char input[] = "\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_32)
{
    const char input[] = "\xE0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -32);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    const char input[] = "\xA0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    const char input[] = "\xA0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x00);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const char input[] = "\xA0\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x01);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    const char input[] = "\xA0\x7F";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    const char input[] = "\xA0\x80";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    const char input[] = "\xA0\x81";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    const char input[] = "\xA0\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    const char input[] = "\xB0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    const char input[] = "\xB0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    const char input[] = "\xB0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const char input[] = "\xB0\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    const char input[] = "\xB0\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    const char input[] = "\xC0\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    const char input[] = "\xC0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    const char input[] = "\xC0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    const char input[] = "\xC0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    const char input[] = "\xC0\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const char input[] = "\xC0\x00\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    const char input[] = "\xC0\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    const char input[] = "\xC0\x00\x00\x00\x02";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    const char input[] = "\xC0\xFF\xFF\xFF\xFE";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    const char input[] = "\xC0\x7F\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    const char input[] = "\xC0\x80\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    const char input[] = "\xD0\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    const char input[] = "\xD0\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    const char input[] = "\xD0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    const char input[] = "\xD0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    const char input[] = "\xD0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    const char input[] = "\xD0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    const char input[] = "\xD0\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    const char input[] = "\xC2\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    const char input[] = "\xC2\x3F\x80\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    const char input[] = "\xC2\xBF\x80\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    const char input[] = "\xC2\x40\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    const char input[] = "\xC2\xC0\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_one)
{
    const char input[] = "\xC2\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_two)
{
    const char input[] = "\xC2\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_three)
{
    const char input[] = "\xC2\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_four)
{
    const char input[] = "\xC2";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    const char input[] = "\xD2\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    const char input[] = "\xD2\x3F\xF0\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    const char input[] = "\xD2\xBF\xF0\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    const char input[] = "\xD2\x40\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    const char input[] = "\xD2\xC0\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_one)
{
    const char input[] = "\xD2\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_two)
{
    const char input[] = "\xD2\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_three)
{
    const char input[] = "\xD2\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_four)
{
    const char input[] = "\xD2\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_five)
{
    const char input[] = "\xD2\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_six)
{
    const char input[] = "\xD2\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_seven)
{
    const char input[] = "\xD2\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_eight)
{
    const char input[] = "\xD2";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}


//-----------------------------------------------------------------------------
// Binary data
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_int8_empty)
{
    const char input[] = "\xA8" "\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int16_empty)
{
    const char input[] = "\xB8" "\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int32_empty)
{
    const char input[] = "\xC8" "\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int64_empty)
{
    const char input[] = "\xD8" "\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int8_one)
{
    const char input[] = "\xA8" "\x01" "\x12";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "\x12");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int16_one)
{
    const char input[] = "\xB8" "\x00\x01" "\x12";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "\x12");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int8_two)
{
    const char input[] = "\xA8" "\x02" "\x12\x34";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_array);
    BOOST_REQUIRE_EQUAL(decoder.get_array(), "\x12\x34");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int8_missing_length)
{
    const char input[] = "\xA8";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int8_missing_content)
{
    const char input[] = "\xA8" "\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_int8_invalid_length)
{
    const char input[] = "\xA8" "\xFF" "\x12";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

BOOST_AUTO_TEST_CASE(test_array_int8_invalid_length_2)
{
    const char input[] = "\xA8" "\xE0" "\x12";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_int8_empty)
{
    const char input[] = "\xA9" "\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int16_empty)
{
    const char input[] = "\xB9" "\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_one)
{
    const char input[] = "\xA9" "\x01" "A";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_two)
{
    const char input[] = "\xA9" "\x02" "AB";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_missing_length_indicator)
{
    const char input[] = "\xA9";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_invalid)
{
    const char input[] = "\xA9" "\xF0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

//-----------------------------------------------------------------------------
// Unknowns
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_unknown_none)
{
    const char input[] = "\x8F";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8)
{
    const char input[] = "\xAF" "\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_missing_one)
{
    const char input[] = "\xAF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
