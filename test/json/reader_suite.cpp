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

#include <protoc/json/reader.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_reader_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    const char input[] = "null";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_null);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "false";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), false);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "true";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_boolean);
    BOOST_REQUIRE_EQUAL(reader.get_bool(), true);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_integer)
{
    const char input[] = "1";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_float)
{
    const char input[] = "1.0";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_floating);
    BOOST_REQUIRE_EQUAL(reader.get_double(), 1.0);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

BOOST_AUTO_TEST_CASE(test_string)
{
    const char input[] = "\"alpha\"";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "alpha");
    BOOST_REQUIRE_EQUAL(reader.next(), false);
}

//-----------------------------------------------------------------------------
// Array
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_empty)
{
    const char input[] = "[]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_integer_array_one)
{
    const char input[] = "[1]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_integer_array_many)
{
    const char input[] = "[1,2,3]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 2);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 3);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_integer_array_nested_one)
{
    const char input[] = "[[1]]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_integer_array_nested_many_siblings)
{
    const char input[] = "[[1],[2]]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 2);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(fail_array_comma)
{
    const char input[] = "[,]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_THROW(reader.next(), unexpected_token);
}

BOOST_AUTO_TEST_CASE(fail_array_missing_value)
{
    const char input[] = "[1,]";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_array_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_THROW(reader.next(), unexpected_token);
}

//-----------------------------------------------------------------------------
// Object
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_object_empty)
{
    const char input[] = "{}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_object_one)
{
    const char input[] = "{\"alpha\":1}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "alpha");
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_object_many)
{
    const char input[] = "{\"alpha\":1,\"bravo\":2}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "alpha");
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 1);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "bravo");
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 2);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_object_nested_one)
{
    const char input[] = "{\"alpha\":{\"helium\":2}}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "alpha");
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.get_string(), "helium");
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_integer);
    BOOST_REQUIRE_EQUAL(reader.get_int(), 2);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 2);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_end);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_EQUAL(reader.next(), false);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_eof);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
}

BOOST_AUTO_TEST_CASE(fail_object_missing_colon)
{
    const char input[] = "{\"key\"}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_THROW(reader.next(), unexpected_token);
}

BOOST_AUTO_TEST_CASE(fail_object_missing_value)
{
    const char input[] = "{\"key\":}";
    json::reader reader(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_map_begin);
    BOOST_REQUIRE_EQUAL(reader.size(), 0);
    BOOST_REQUIRE_EQUAL(reader.next(), true);
    BOOST_REQUIRE_EQUAL(reader.type(), token::token_string);
    BOOST_REQUIRE_EQUAL(reader.size(), 1);
    BOOST_REQUIRE_THROW(reader.next(), unexpected_token);
}

BOOST_AUTO_TEST_SUITE_END()
