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
#include <protoc/transenc/detail/decoder.hpp>
#include <protoc/transenc/detail/codes.hpp>

namespace format = protoc::transenc::detail;
namespace detail = format;

BOOST_AUTO_TEST_SUITE(transenc_decoder_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    format::decoder::value_type input[] = {};
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    format::decoder::value_type input[] = { detail::code_false };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_false);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    format::decoder::value_type input[] = { detail::code_true };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_true);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    format::decoder::value_type input[] = { detail::code_null };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_small_zero)
{
    format::decoder::value_type input[] = { 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_one)
{
    format::decoder::value_type input[] = { 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_127)
{
    format::decoder::value_type input[] = { 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_one)
{
    format::decoder::value_type input[] = { 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_small_minus_32)
{
    format::decoder::value_type input[] = { 0xE0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -32);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_missing_one)
{
    format::decoder::value_type input[] = { detail::code_int8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    format::decoder::value_type input[] = { detail::code_int8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x00);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    format::decoder::value_type input[] = { detail::code_int8, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 0x01);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_127)
{
    format::decoder::value_type input[] = { detail::code_int8, 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), 127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_128)
{
    format::decoder::value_type input[] = { detail::code_int8, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -128);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_127)
{
    format::decoder::value_type input[] = { detail::code_int8, 0x81 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -127);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    format::decoder::value_type input[] = { detail::code_int8, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int8);
    BOOST_REQUIRE_EQUAL(decoder.get_int8(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_one)
{
    format::decoder::value_type input[] = { detail::code_int16, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_missing_two)
{
    format::decoder::value_type input[] = { detail::code_int16 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_zero)
{
    format::decoder::value_type input[] = { detail::code_int16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_one)
{
    format::decoder::value_type input[] = { detail::code_int16, 0x01, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), 0x0001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int16_minus_one)
{
    format::decoder::value_type input[] = { detail::code_int16, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int16);
    BOOST_REQUIRE_EQUAL(decoder.get_int16(), -1);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_one)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_two)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_three)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_missing_four)
{
    format::decoder::value_type input[] = { detail::code_int32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_zero)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000000);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_one)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x01, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_one)
{
    format::decoder::value_type input[] = { detail::code_int32, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000001);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_two)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x02, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_two)
{
    format::decoder::value_type input[] = { detail::code_int32, 0xFE, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x00000002);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_7FFFFFFF)
{
    format::decoder::value_type input[] = { detail::code_int32, 0xFF, 0xFF, 0xFF, 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), 0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int32_minus_7FFFFFFF)
{
    format::decoder::value_type input[] = { detail::code_int32, 0x01, 0x00, 0x00, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int32);
    BOOST_REQUIRE_EQUAL(decoder.get_int32(), -0x7FFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_one)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_two)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_three)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_four)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_five)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_six)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_seven)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_missing_eight)
{
    format::decoder::value_type input[] = { detail::code_int64 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_zero)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000000L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_one)
{
    format::decoder::value_type input[] = { detail::code_int64, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_minus_one)
{
    format::decoder::value_type input[] = { detail::code_int64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), -0x0000000000000001L);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_int64_7FFFFFFFFFFFFFFF)
{
    format::decoder::value_type input[] = { detail::code_int64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_int64);
    BOOST_REQUIRE_EQUAL(decoder.get_int64(), 0x7FFFFFFFFFFFFFFF);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float32_zero)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 0.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_one)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x80, 0x3F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_one)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x80, 0xBF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -1.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_two)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x00, 0x40 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), 2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_minus_two)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x00, 0xC0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float32);
    BOOST_REQUIRE_EQUAL(decoder.get_float32(), -2.0f);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_one)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_two)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_three)
{
    format::decoder::value_type input[] = { detail::code_float32, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float32_missing_four)
{
    format::decoder::value_type input[] = { detail::code_float32 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_zero)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 0.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_one)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_one)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xBF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -1.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_two)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), 2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_minus_two)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_float64);
    BOOST_REQUIRE_EQUAL(decoder.get_float64(), -2.0);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_one)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_two)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_three)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_four)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_five)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_six)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_seven)
{
    format::decoder::value_type input[] = { detail::code_float64, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_float64_missing_eight)
{
    format::decoder::value_type input[] = { detail::code_float64 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}


//-----------------------------------------------------------------------------
// Binary data
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_binary_int8_empty)
{
    format::decoder::value_type input[] = { detail::code_binary_int8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {};
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int16_empty)
{
    format::decoder::value_type input[] = { detail::code_binary_int16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {};
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int32_empty)
{
    format::decoder::value_type input[] = { detail::code_binary_int32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {};
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int64_empty)
{
    format::decoder::value_type input[] = { detail::code_binary_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {};
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_one)
{
    format::decoder::value_type input[] = { detail::code_binary_int8, 0x01, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {
        0x12
    };
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int16_one)
{
    format::decoder::value_type input[] = { detail::code_binary_int16, 0x01, 0x00, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {
        0x12
    };
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_two)
{
    format::decoder::value_type input[] = { detail::code_binary_int8, 0x02, 0x12, 0x34 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::value_type expected[] = {
        0x12, 0x34
    };
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL_COLLECTIONS(range.begin(), range.end(),
                                    expected, expected + sizeof(expected));
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_128)
{
    unsigned char input[2 + 0x80] = { detail::code_binary_int8, 0x80 };
    std::fill_n(&input[2], sizeof(input) - 2, 0x12);
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_binary);
    format::decoder::input_range range = decoder.get_range();
    BOOST_REQUIRE_EQUAL(range.size(), 0x80);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_missing_length)
{
    format::decoder::value_type input[] = { detail::code_binary_int8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int8_missing_content)
{
    format::decoder::value_type input[] = { detail::code_binary_int8, 0x01 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_binary_int64_invalid_length)
{
    format::decoder::value_type input[] = { detail::code_binary_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_error);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_int8_empty)
{
    format::decoder::value_type input[] = { detail::code_string_int8, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int16_empty)
{
    format::decoder::value_type input[] = { detail::code_string_int16, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int32_empty)
{
    format::decoder::value_type input[] = { detail::code_string_int32, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int64_empty)
{
    format::decoder::value_type input[] = { detail::code_string_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_one)
{
    format::decoder::value_type input[] = { detail::code_string_int8, 0x01, 'A' };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "A");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_two)
{
    format::decoder::value_type input[] = { detail::code_string_int8, 0x02, 'A', 'B' };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_string);
    BOOST_REQUIRE_EQUAL(decoder.get_string(), "AB");
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_missing_length_indicator)
{
    format::decoder::value_type input[] = { detail::code_string_int8 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_string_int8_invalid_length)
{
    format::decoder::value_type input[] = { detail::code_string_int64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_error);
}

//-----------------------------------------------------------------------------
// Unknowns
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_unknown_none)
{
    format::decoder::value_type input[] = { 0x8F };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8)
{
    format::decoder::value_type input[] = { 0xA7, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_missing_one)
{
    format::decoder::value_type input[] = { 0xA7 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_empty)
{
    format::decoder::value_type input[] = { 0xAF, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_one)
{
    format::decoder::value_type input[] = { 0xAF, 0x01, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int8_length_missing_one)
{
    format::decoder::value_type input[] = { 0xAF };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16)
{
    format::decoder::value_type input[] = { 0xB7, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_missing_one)
{
    format::decoder::value_type input[] = { 0xB7, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_empty)
{
    format::decoder::value_type input[] = { 0xBF, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_one)
{
    format::decoder::value_type input[] = { 0xBF, 0x01, 0x00, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int16_length_missing_one)
{
    format::decoder::value_type input[] = { 0xBF, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32)
{
    format::decoder::value_type input[] = { 0xC7, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_missing_one)
{
    format::decoder::value_type input[] = { 0xC7, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_empty)
{
    format::decoder::value_type input[] = { 0xCF, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_one)
{
    format::decoder::value_type input[] = { 0xCF, 0x01, 0x00, 0x00, 0x00, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int32_length_missing_one)
{
    format::decoder::value_type input[] = { 0xCF, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64)
{
    format::decoder::value_type input[] = { 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_missing_one)
{
    format::decoder::value_type input[] = { 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_empty)
{
    format::decoder::value_type input[] = { 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_one)
{
    format::decoder::value_type input[] = { 0xDF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_CASE(test_unknown_int64_length_missing_one)
{
    format::decoder::value_type input[] = { 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::decoder decoder(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(decoder.type(), format::token_eof);
}

BOOST_AUTO_TEST_SUITE_END()
