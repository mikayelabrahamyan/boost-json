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
#include <protoc/transenc/codes.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_reader_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    transenc::reader::value_type input[] = {};
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    transenc::reader::value_type input[] = { transenc::code_false };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), false);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    transenc::reader::value_type input[] = { transenc::code_true };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), true);
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_null)
{
    transenc::reader::value_type input[] = { transenc::code_null };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_null);
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_integer)
{
    transenc::reader::value_type input[] = { 0x01 };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE(!reader.next());
}

BOOST_AUTO_TEST_CASE(test_floating)
{
    transenc::reader::value_type input[] = { transenc::code_float64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_floating);
    BOOST_REQUIRE_EQUAL(reader.get_double(), 1.0);
    BOOST_REQUIRE(!reader.next());
}

//-----------------------------------------------------------------------------
// Containers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_record)
{
    transenc::reader::value_type input[] = { transenc::code_record_begin, transenc::code_record_end };
    transenc::reader reader(input, input + sizeof(input));
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_record_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE(reader.next());
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_record_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE(!reader.next());
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
