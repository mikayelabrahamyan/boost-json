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

#include <protoc/ubjson/decoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(ubjson_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_junk)
{
    const char input[] = "\t";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_error);
}

BOOST_AUTO_TEST_CASE(test_noop)
{
    const char input[] = "NNN";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    const char input[] = "Z";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_null);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "T";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_true);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "F";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_false);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    const char input[] = "B";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    const char input[] = "B\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x00);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const char input[] = "B\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x01);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_7F)
{
    const char input[] = "B\x7F";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x7F);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_80)
{
    const char input[] = "B\x80";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -0x80);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_81)
{
    // Integers as signed (two's complement)
    const char input[] = "B\x81";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -0x7F);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_FF)
{
    const char input[] = "B\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    const char input[] = "i\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    const char input[] = "i";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    const char input[] = "i\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const char input[] = "i\x00\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    const char input[] = "i\xFF\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -0x0001);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_FF)
{
    const char input[] = "i\x00\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x00FF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_7FFF)
{
    const char input[] = "i\x7F\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x7FFF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_7FFF)
{
    const char input[] = "i\x80\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -0x7FFF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_8000)
{
    const char input[] = "i\x80\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -0x8000);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    const char input[] = "I\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    const char input[] = "I\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    const char input[] = "I\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    const char input[] = "I";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    const char input[] = "I\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const char input[] = "I\x00\x00\x00\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    const char input[] = "I\xFF\xFF\xFF\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    const char input[] = "I\x00\x00\x00\x02";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    const char input[] = "I\xFF\xFF\xFF\xFE";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    const char input[] = "I\x7F\xFF\xFF\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    const char input[] = "I\x80\x00\x00\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    const char input[] = "L\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    const char input[] = "L\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    const char input[] = "L\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    const char input[] = "L\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    const char input[] = "L\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    const char input[] = "L\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    const char input[] = "L\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    const char input[] = "L";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    const char input[] = "L\x00\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const char input[] = "L\x00\x00\x00\x00\x00\x00\x00\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    const char input[] = "L\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    const char input[] = "L\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    const char input[] = "d" "\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    const char input[] = "d" "\x3F\x80\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    const char input[] = "d" "\xBF\x80\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    const char input[] = "d" "\x40\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    const char input[] = "d" "\xC0\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

#if 0
BOOST_AUTO_TEST_CASE(test_float32_e14)
{
    const char input[] = "d" "\x56\xB5\xE6\x21"; // FIXME: Find exact number
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1e14);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_em14)
{
    const char input[] = "d" "\x28\x34\x24\xDC"; // FIXME: Find exact number
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1e-14);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}
#endif

BOOST_AUTO_TEST_CASE(test_float32_missing_one)
{
    const char input[] = "d" "\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_two)
{
    const char input[] = "d" "\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_three)
{
    const char input[] = "d" "\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_four)
{
    const char input[] = "d";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    const char input[] = "D" "\x00\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    const char input[] = "D" "\x3F\xF0\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    const char input[] = "D" "\xBF\xF0\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    const char input[] = "D" "\x40\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    const char input[] = "D" "\xC0\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_one)
{
    const char input[] = "D" "\x00\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_two)
{
    const char input[] = "D" "\x00\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_three)
{
    const char input[] = "D" "\x00\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_four)
{
    const char input[] = "D" "\x00\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_five)
{
    const char input[] = "D" "\x00\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_six)
{
    const char input[] = "D" "\x00\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_seven)
{
    const char input[] = "D" "\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_eight)
{
    const char input[] = "D";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    const char input[] = "s" "B\x00";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_one)
{
    const char input[] = "s" "B\x01" "A";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_two)
{
    const char input[] = "s" "B\x02" "AB";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_missing_length_indicator)
{
    const char input[] = "s";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_invalid)
{
    const char input[] = "s" "s";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_missing_length)
{
    const char input[] = "s" "B";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_missing_data)
{
    const char input[] = "s" "B\x01";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_length_too_big)
{
    const char input[] = "s" "B\x02" "A";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_negative_length_FF)
{
    const char input[] = "s" "B\xFF" "A";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_negative_length_80)
{
    const char input[] = "s" "B\x80" "A";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_error);
}

//-----------------------------------------------------------------------------
// Containers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_object_begin)
{
    const char input[] = "{";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_object_end)
{
    const char input[] = "}";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_object_empty)
{
    const char input[] = "{}";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_object_nested_empty)
{
    const char input[] = "{{}}";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_object_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_begin)
{
    const char input[] = "[";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_end)
{
    const char input[] = "]";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_empty)
{
    const char input[] = "[]";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array_nested_empty)
{
    const char input[] = "[[]]";
    ubjson::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_begin);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_array_end);
    BOOST_REQUIRE_NO_THROW(decoder.next());
    BOOST_REQUIRE_EQUAL(decoder.type(), ubjson::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
