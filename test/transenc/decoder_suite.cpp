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

BOOST_AUTO_TEST_CASE(test_small_minus_16)
{
    const char input[] = "\xF0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -16);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    const char input[] = "\x90";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    const char input[] = "\x90\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x00);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const char input[] = "\x90\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x01);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    const char input[] = "\x90\x7F";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    const char input[] = "\x90\x80";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    const char input[] = "\x90\x81";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    const char input[] = "\x90\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    const char input[] = "\xA0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    const char input[] = "\xA0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    const char input[] = "\xA0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const char input[] = "\xA0\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    const char input[] = "\xA0\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    const char input[] = "\xB0\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    const char input[] = "\xB0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    const char input[] = "\xB0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    const char input[] = "\xB0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    const char input[] = "\xB0\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const char input[] = "\xB0\x00\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    const char input[] = "\xB0\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    const char input[] = "\xB0\x00\x00\x00\x02";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    const char input[] = "\xB0\xFF\xFF\xFF\xFE";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    const char input[] = "\xB0\x7F\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    const char input[] = "\xB0\x80\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    const char input[] = "\xC0\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    const char input[] = "\xC0\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    const char input[] = "\xC0\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    const char input[] = "\xC0\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    const char input[] = "\xC0\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    const char input[] = "\xC0\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    const char input[] = "\xC0\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    const char input[] = "\xC0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    const char input[] = "\xC0\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const char input[] = "\xC0\x00\x00\x00\x00\x00\x00\x00\x01";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    const char input[] = "\xC0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    const char input[] = "\xC0\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    const char input[] = "\xE3" "\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_one)
{
    const char input[] = "\xE3" "\x01" "A";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_two)
{
    const char input[] = "\xE3" "\x02" "AB";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_missing_length_indicator)
{
    const char input[] = "\xE3";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_invalid)
{
    const char input[] = "\xE3" "\xF0";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_invalid_false)
{
    const char input[] = "\xE3" "\x80";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_invalid_true)
{
    const char input[] = "\xE3" "\x81";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_invalid_null)
{
    const char input[] = "\xE3" "\x82";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_error);
}

BOOST_AUTO_TEST_CASE(test_string_int8_empty)
{
    const char input[] = "\xE3" "\x90\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
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

BOOST_AUTO_TEST_CASE(test_unknown_int128)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int128_missing_one)
{
    const char input[] = "\xD0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    transenc::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), transenc::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
