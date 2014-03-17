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

#include <protoc/transenc/reader.hpp>
#include <protoc/transenc/detail/codes.hpp>

namespace format = protoc::transenc;
namespace detail = format::detail;

BOOST_AUTO_TEST_SUITE(transenc_reader_suite)

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
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_null)
{
    format::reader::value_type input[] = { detail::code_null };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_null);
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_integer)
{
    format::reader::value_type input[] = { 0x01 };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_floating)
{
    format::reader::value_type input[] = { detail::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_floating);
    BOOST_REQUIRE_EQUAL(reader.get_double(), 1.0);
    BOOST_REQUIRE(!reader.next());
}

//-----------------------------------------------------------------------------
// Containers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_record)
{
    format::reader::value_type input[] = { detail::code_record_begin, detail::code_record_end };
    format::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_record_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE(reader.next());
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_record_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE(!reader.next());
    BOOST_REQUIRE_EQUAL(reader.type(), protoc::token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
