///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <protoc/msgpack/reader.hpp>
#include <protoc/msgpack/detail/codes.hpp>

namespace format = protoc::msgpack;
namespace detail = protoc::msgpack::detail;

BOOST_AUTO_TEST_SUITE(msgpack_reader_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    format::reader::value_type input[] = {};
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    format::reader::value_type input[] = { detail::code_false };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), false);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    format::reader::value_type input[] = { detail::code_true };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), true);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    format::reader::value_type input[] = { detail::code_null };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_null);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_integer)
{
    format::reader::value_type input[] = { 0x01 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_floating)
{
    format::reader::value_type input[] = { detail::code_float64, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_floating);
    BOOST_REQUIRE_EQUAL(reader.get_double(), 1.0);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

//-----------------------------------------------------------------------------
// Strings
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string)
{
    format::reader::value_type input[] = { detail::code_str16, 0x00, 0x01, 0x41 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "A");
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_fixstring)
{
    format::reader::value_type input[] = { detail::code_fixstr_1, 0x41 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "A");
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

//-----------------------------------------------------------------------------
// Containers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array16_empty)
{
    format::reader::value_type input[] = { detail::code_array16, 0x00, 0x00 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1U);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_array16_one)
{
    format::reader::value_type input[] = { detail::code_array16, 0x00, 0x01, detail::code_null };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_null);
    BOOST_REQUIRE_EQUAL(reader.size(), 1U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_array16_nested_one)
{
    format::reader::value_type input[] = { detail::code_array16, 0x00, 0x01, detail::code_array16, 0x00, 0x01, detail::code_null };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 1U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_null);
    BOOST_REQUIRE_EQUAL(reader.size(), 2U);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.size(), 1U);
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_SUITE_END()
