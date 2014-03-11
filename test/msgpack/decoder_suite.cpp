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

#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/detail/decoder.hpp>

namespace format = protoc::msgpack::detail;

BOOST_AUTO_TEST_SUITE(msgpack_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    format::decoder::value_type input[] = {};
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    format::decoder::value_type input[] = { format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    format::decoder::value_type input[] = { format::code_false };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_false);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    format::decoder::value_type input[] = { format::code_true };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_true);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_posfixnum_zero)
{
    format::decoder::value_type input[] = { 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_posfixnum_one)
{
    format::decoder::value_type input[] = { 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_posfixnum_max)
{
    format::decoder::value_type input[] = { 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_negfixnum_one)
{
    format::decoder::value_type input[] = { 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_negfixnum_max)
{
    format::decoder::value_type input[] = { 0xE0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -32);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    format::decoder::value_type input[] = { format::code_int8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    format::decoder::value_type input[] = { format::code_int8, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_max)
{
    format::decoder::value_type input[] = { format::code_int8, 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    format::decoder::value_type input[] = { format::code_int8, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    format::decoder::value_type input[] = { format::code_int8, 0x81 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    format::decoder::value_type input[] = { format::code_int8, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int8_missing_one)
{
    format::decoder::value_type input[] = { format::code_int8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    format::decoder::value_type input[] = { format::code_int16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    format::decoder::value_type input[] = { format::code_int16, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    format::decoder::value_type input[] = { format::code_int16, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int16_missing_one)
{
    format::decoder::value_type input[] = { format::code_int16, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int16_missing_two)
{
    format::decoder::value_type input[] = { format::code_int16 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00, 0x00, 0x00, 0x02 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    format::decoder::value_type input[] = { format::code_int32, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    format::decoder::value_type input[] = { format::code_int32, 0xFF, 0xFF, 0xFF, 0xFE };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_max)
{
    format::decoder::value_type input[] = { format::code_int32, 0x7F, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_max)
{
    format::decoder::value_type input[] = { format::code_int32, 0x80, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int32_missing_one)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int32_missing_two)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int32_missing_three)
{
    format::decoder::value_type input[] = { format::code_int32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_int32_missing_four)
{
    format::decoder::value_type input[] = { format::code_int32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    format::decoder::value_type input[] = { format::code_int64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_max)
{
    format::decoder::value_type input[] = { format::code_int64, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    format::decoder::value_type input[] = { format::code_int64, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    format::decoder::value_type input[] = { format::code_int64 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_zero)
{
    format::decoder::value_type input[] = { format::code_uint8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_one)
{
    format::decoder::value_type input[] = { format::code_uint8, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_127)
{
    format::decoder::value_type input[] = { format::code_uint8, 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_128)
{
    format::decoder::value_type input[] = { format::code_uint8, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 128);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_129)
{
    format::decoder::value_type input[] = { format::code_uint8, 0x81 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 129);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint8_255)
{
    format::decoder::value_type input[] = { format::code_uint8, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint8);
    BOOST_REQUIRE_EQUAL(decoder.get_uint8(), 255);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint8_missing_one)
{
    format::decoder::value_type input[] = { format::code_uint8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_zero)
{
    format::decoder::value_type input[] = { format::code_uint16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0x0000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_one)
{
    format::decoder::value_type input[] = { format::code_uint16, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0x0001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint16_max)
{
    format::decoder::value_type input[] = { format::code_uint16, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint16);
    BOOST_REQUIRE_EQUAL(decoder.get_uint16(), 0xFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint16_missing_one)
{
    format::decoder::value_type input[] = { format::code_uint16, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint16_missing_two)
{
    format::decoder::value_type input[] = { format::code_uint16 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_zero)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_one)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_two)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00, 0x00, 0x00, 0x02 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_max)
{
    format::decoder::value_type input[] = { format::code_uint32, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0xFFFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_below_max)
{
    format::decoder::value_type input[] = { format::code_uint32, 0xFF, 0xFF, 0xFF, 0xFE };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0xFFFFFFFE);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_below_middle)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x7F, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_middle)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x80, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x80000000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint32_above_middle)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x80, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint32);
    BOOST_REQUIRE_EQUAL(decoder.get_uint32(), 0x80000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint32_missing_one)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint32_missing_two)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint32_missing_three)
{
    format::decoder::value_type input[] = { format::code_uint32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint32_missing_four)
{
    format::decoder::value_type input[] = { format::code_uint32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_zero)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x0000000000000000L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_one)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_max)
{
    format::decoder::value_type input[] = { format::code_uint64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0xFFFFFFFFFFFFFFFFL);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_uint64_below_middle)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_uint64);
    BOOST_REQUIRE_EQUAL(decoder.get_uint64(), 0x7FFFFFFFFFFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_one)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_two)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_three)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_four)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_five)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_six)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_seven)
{
    format::decoder::value_type input[] = { format::code_uint64, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_uint64_missing_eight)
{
    format::decoder::value_type input[] = { format::code_uint64 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    format::decoder::value_type input[] = { format::code_float32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    format::decoder::value_type input[] = { format::code_float32, 0x3F, 0x80, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    format::decoder::value_type input[] = { format::code_float32, 0x40, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    format::decoder::value_type input[] = { format::code_float32, 0xBF, 0x80, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    format::decoder::value_type input[] = { format::code_float32, 0xC0, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float32_missing_one)
{
    format::decoder::value_type input[] = { format::code_float32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float32_missing_two)
{
    format::decoder::value_type input[] = { format::code_float32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float32_missing_three)
{
    format::decoder::value_type input[] = { format::code_float32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float32_missing_four)
{
    format::decoder::value_type input[] = { format::code_float32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    format::decoder::value_type input[] = { format::code_float64, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    format::decoder::value_type input[] = { format::code_float64, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    format::decoder::value_type input[] = { format::code_float64, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    format::decoder::value_type input[] = { format::code_float64, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_one)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_two)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_three)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_four)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_five)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_six)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_seven)
{
    format::decoder::value_type input[] = { format::code_float64, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_float64_missing_eight)
{
    format::decoder::value_type input[] = { format::code_float64 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Fixed string
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_fixstr_0)
{
    format::decoder::value_type input[] = { format::code_fixstr_0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixstr_1)
{
    format::decoder::value_type input[] = { format::code_fixstr_1, 0x41 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_fixstr_1_missing_one)
{
    format::decoder::value_type input[] = { format::code_fixstr_1 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixstr_2)
{
    format::decoder::value_type input[] = { format::code_fixstr_2, 0x41, 0x42 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixstr_15)
{
    format::decoder::value_type input[] = { format::code_fixstr_15, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "ABCDEFGHIJKLMNO");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_fixstr_15_missing_one)
{
    format::decoder::value_type input[] = { format::code_fixstr_15, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E };
    format::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixstr_max)
{
    format::decoder::value_type input[] = { format::code_fixstr_31, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcde");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_str8_empty)
{
    format::decoder::value_type input[] = { format::code_str8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str8_one)
{
    format::decoder::value_type input[] = { format::code_str8, 0x01, 0x41 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str8_two)
{
    format::decoder::value_type input[] = { format::code_str8, 0x02, 0x41, 0x42 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str8);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str8_missing_one)
{
    format::decoder::value_type input[] = { format::code_str8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str16_empty)
{
    format::decoder::value_type input[] = { format::code_str16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str16);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str16_one)
{
    format::decoder::value_type input[] = { format::code_str16, 0x00, 0x01, 0x41 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str16);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str16_missing_one)
{
    format::decoder::value_type input[] = { format::code_str16, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str16_missing_two)
{
    format::decoder::value_type input[] = { format::code_str16 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str32_empty)
{
    format::decoder::value_type input[] = { format::code_str32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str32);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_str32_one)
{
    format::decoder::value_type input[] = { format::code_str32, 0x00, 0x00, 0x00, 0x01, 0x41 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_str32);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str32_missing_one)
{
    format::decoder::value_type input[] = { format::code_str32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str32_missing_two)
{
    format::decoder::value_type input[] = { format::code_str32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str32_missing_three)
{
    format::decoder::value_type input[] = { format::code_str32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_str32_missing_four)
{
    format::decoder::value_type input[] = { format::code_str32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Binary
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_bin8_empty)
{
    format::decoder::value_type input[] = { format::code_bin8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_bin8);
    format::decoder::value_type expected[] = {};
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Fixed array
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_fixarray_0)
{
    format::decoder::value_type input[] = { format::code_fixarray_0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_1)
{
    format::decoder::value_type input[] = { format::code_fixarray_1, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_1_missing_one)
{
    format::decoder::value_type input[] = { format::code_fixarray_1 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_2)
{
    format::decoder::value_type input[] = { format::code_fixarray_2, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 2U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_3)
{
    format::decoder::value_type input[] = { format::code_fixarray_3, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 3U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_4)
{
    format::decoder::value_type input[] = { format::code_fixarray_4, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 4U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_5)
{
    format::decoder::value_type input[] = { format::code_fixarray_5, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 5U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_6)
{
    format::decoder::value_type input[] = { format::code_fixarray_6, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 6U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_7)
{
    format::decoder::value_type input[] = { format::code_fixarray_7, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 7U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_8)
{
    format::decoder::value_type input[] = { format::code_fixarray_8, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 8U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_9)
{
    format::decoder::value_type input[] = { format::code_fixarray_9, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 9U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_10)
{
    format::decoder::value_type input[] = { format::code_fixarray_10, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 10U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_11)
{
    format::decoder::value_type input[] = { format::code_fixarray_11, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 11U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_12)
{
    format::decoder::value_type input[] = { format::code_fixarray_12, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 12U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_13)
{
    format::decoder::value_type input[] = { format::code_fixarray_13, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 13U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_14)
{
    format::decoder::value_type input[] = { format::code_fixarray_14, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 14U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fixarray_15)
{
    format::decoder::value_type input[] = { format::code_fixarray_15, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array8);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 15U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Array
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array16_empty)
{
    format::decoder::value_type input[] = { format::code_array16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array16);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array16_one)
{
    format::decoder::value_type input[] = { format::code_array16, 0x00, 0x01, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array16);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array16_missing_one)
{
    format::decoder::value_type input[] = { format::code_array16, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array16_missing_two)
{
    format::decoder::value_type input[] = { format::code_array16 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_empty)
{
    format::decoder::value_type input[] = { format::code_array32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array32);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 0U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_array32_one)
{
    format::decoder::value_type input[] = { format::code_array32, 0x00, 0x00, 0x00, 0x01, format::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_array32);
    BOOST_REQUIRE_EQUAL(decoder.get_count(), 1U);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array32_missing_one)
{
    format::decoder::value_type input[] = { format::code_array32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array32_missing_two)
{
    format::decoder::value_type input[] = { format::code_array32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array32_missing_three)
{
    format::decoder::value_type input[] = { format::code_array32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(fail_array32_missing_four)
{
    format::decoder::value_type input[] = { format::code_array32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
