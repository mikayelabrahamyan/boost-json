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

#include <protoc/msgpack/decoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(msgpack_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    const char input[] = "\xC0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "\xC2";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_false);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "\xC3";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_true);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_posfixnum_zero)
{
    const char input[] = "\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_posfixnum_one)
{
    const char input[] = "\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_posfixnum_127)
{
    const char input[] = "\x7F";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_negfixnum_one)
{
    const char input[] = "\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_negfixnum_32)
{
    const char input[] = "\xE0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -32);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    const char input[] = "\xD0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    const char input[] = "\xD0\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    const char input[] = "\xD0\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    const char input[] = "\xD0\x7F";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    const char input[] = "\xD0\x80";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    const char input[] = "\xD0\x81";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    const char input[] = "\xD0\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    const char input[] = "\xD1\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    const char input[] = "\xD1";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    const char input[] = "\xD1\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    const char input[] = "\xD1\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    const char input[] = "\xD1\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    const char input[] = "\xD2\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    const char input[] = "\xD2\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    const char input[] = "\xD2\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    const char input[] = "\xD2";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    const char input[] = "\xD2\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    const char input[] = "\xD2\x00\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    const char input[] = "\xD2\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    const char input[] = "\xD2\x00\x00\x00\x02";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    const char input[] = "\xD2\xFF\xFF\xFF\xFE";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    const char input[] = "\xD2\x7F\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    const char input[] = "\xD2\x80\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    const char input[] = "\xD3\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    const char input[] = "\xD3\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    const char input[] = "\xD3\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    const char input[] = "\xD3\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    const char input[] = "\xD3\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    const char input[] = "\xD3\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    const char input[] = "\xD3\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    const char input[] = "\xD3";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    const char input[] = "\xD3\x00\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    const char input[] = "\xD3\x00\x00\x00\x00\x00\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    const char input[] = "\xD3\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    const char input[] = "\xD3\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_missing_one)
{
    const char input[] = "\xCC";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_zero)
{
    const char input[] = "\xCC\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_one)
{
    const char input[] = "\xCC\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_127)
{
    const char input[] = "\xCC\x7F";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 127);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_128)
{
    const char input[] = "\xCC\x80";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 128);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_129)
{
    const char input[] = "\xCC\x81";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 129);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_255)
{
    const char input[] = "\xCC\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 255);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_missing_one)
{
    const char input[] = "\xCD\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_missing_two)
{
    const char input[] = "\xCD";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_zero)
{
    const char input[] = "\xCD\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0x0000);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_one)
{
    const char input[] = "\xCD\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0x0001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_FFFF)
{
    const char input[] = "\xCD\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0xFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_missing_one)
{
    const char input[] = "\xCE\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_missing_two)
{
    const char input[] = "\xCE\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_missing_three)
{
    const char input[] = "\xCE\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_missing_four)
{
    const char input[] = "\xCE";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_zero)
{
    const char input[] = "\xCE\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000000);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_one)
{
    const char input[] = "\xCE\x00\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_two)
{
    const char input[] = "\xCE\x00\x00\x00\x02";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000002);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_FFFFFFFF)
{
    const char input[] = "\xCE\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0xFFFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_FFFFFFFE)
{
    const char input[] = "\xCE\xFF\xFF\xFF\xFE";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0xFFFFFFFE);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_7FFFFFFF)
{
    const char input[] = "\xCE\x7F\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x7FFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_minus_80000001)
{
    const char input[] = "\xCE\x80\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x80000001);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_one)
{
    const char input[] = "\xCF\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_two)
{
    const char input[] = "\xCF\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_three)
{
    const char input[] = "\xCF\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_four)
{
    const char input[] = "\xCF\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_five)
{
    const char input[] = "\xCF\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_six)
{
    const char input[] = "\xCF\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_seven)
{
    const char input[] = "\xCF\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_missing_eight)
{
    const char input[] = "\xCF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_zero)
{
    const char input[] = "\xCF\x00\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x0000000000000000L);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_one)
{
    const char input[] = "\xCF\x00\x00\x00\x00\x00\x00\x00\x01";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x0000000000000001L);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_FFFFFFFFFFFFFFFF)
{
    const char input[] = "\xCF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0xFFFFFFFFFFFFFFFFL);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_7FFFFFFFFFFFFFFF)
{
    const char input[] = "\xCF\x7F\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x7FFFFFFFFFFFFFFF);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    const char input[] = "\xCA\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    const char input[] = "\xCA\x3F\x80\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    const char input[] = "\xCA\xBF\x80\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    const char input[] = "\xCA\x40\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    const char input[] = "\xCA\xC0\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_one)
{
    const char input[] = "\xCA\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_two)
{
    const char input[] = "\xCA\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_three)
{
    const char input[] = "\xCA\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_four)
{
    const char input[] = "\xCA";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    const char input[] = "\xCB\x00\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    const char input[] = "\xCB\x3F\xF0\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    const char input[] = "\xCB\xBF\xF0\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    const char input[] = "\xCB\x40\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    const char input[] = "\xCB\xC0\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_one)
{
    const char input[] = "\xCB\x00\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_two)
{
    const char input[] = "\xCB\x00\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_three)
{
    const char input[] = "\xCB\x00\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_four)
{
    const char input[] = "\xCB\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_five)
{
    const char input[] = "\xCB\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_six)
{
    const char input[] = "\xCB\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_seven)
{
    const char input[] = "\xCB\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_eight)
{
    const char input[] = "\xCB";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

//-----------------------------------------------------------------------------
// Raw
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_raw8_empty)
{
    const char input[] = "\xA0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw8);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw8_missing_one)
{
    const char input[] = "\xA1";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw8_one)
{
    const char input[] = "\xA1\x12";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw8);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 1U);
    BOOST_REQUIRE_EQUAL(container[0], 0x12);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw8_two)
{
    const char input[] = "\xA2\x12\x34";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw8);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 2U);
    BOOST_REQUIRE_EQUAL(container[0], 0x12);
    BOOST_REQUIRE_EQUAL(container[1], 0x34);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw8_max_missing_one)
{
    const char input[] = "\xBF\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw8_max)
{
    const char input[] = "\xBF\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12\x34\x12";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw8);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 31U);
    BOOST_REQUIRE_EQUAL(container[0], 0x12);
    BOOST_REQUIRE_EQUAL(container[1], 0x34);
    BOOST_REQUIRE_EQUAL(container[30], 0x12);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw16_missing_one)
{
    const char input[] = "\xDA\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw16_missing_two)
{
    const char input[] = "\xDA";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw16_empty)
{
    const char input[] = "\xDA\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw16);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw16_one)
{
    const char input[] = "\xDA\x00\x01\x12";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw16);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 1U);
    BOOST_REQUIRE_EQUAL(container[0], 0x12);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_missing_one)
{
    const char input[] = "\xDB\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_missing_two)
{
    const char input[] = "\xDB\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_missing_three)
{
    const char input[] = "\xDB\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_missing_four)
{
    const char input[] = "\xDB";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_empty)
{
    const char input[] = "\xDB\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw32);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_raw32_one)
{
    const char input[] = "\xDB\x00\x00\x00\x01\x12";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_raw32);
    std::vector<protoc::uint8_t> container = decoder.get_raw();
    BOOST_REQUIRE_EQUAL(container.size(), 1U);
    BOOST_REQUIRE_EQUAL(container[0], 0x12);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

//-----------------------------------------------------------------------------
// Array
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array8_empty)
{
    const char input[] = "\x90";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array8_missing_one)
{
    const char input[] = "\x91";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array8_one)
{
    const char input[] = "\x91\xC0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array8_two)
{
    const char input[] = "\x92\xC0\xC0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 2U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array16_missing_one)
{
    const char input[] = "\xDC\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array16_missing_two)
{
    const char input[] = "\xDC";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array16_empty)
{
    const char input[] = "\xDC\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array16);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array16_one)
{
    const char input[] = "\xDC\x00\x01\xC0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array16);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_missing_one)
{
    const char input[] = "\xDD\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_missing_two)
{
    const char input[] = "\xDD\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_missing_three)
{
    const char input[] = "\xDD\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_missing_four)
{
    const char input[] = "\xDD";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_empty)
{
    const char input[] = "\xDD\x00\x00\x00\x00";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array32);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_one)
{
    const char input[] = "\xDD\x00\x00\x00\x01\xC0";
    msgpack::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_array32);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_null);
    BOOST_REQUIRE_EQUAL(decoder.next(), msgpack::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
