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

#include <protoc/json/decoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_decoder_suite)

//-----------------------------------------------------------------------------
// Whitespaces
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_empty)
{
    const char input[] = "";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_spaces)
{
    const char input[] = "  ";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_tabs)
{
    const char input[] = "\t\t";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_carriage_returns)
{
    const char input[] = "\r\r";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_newlines)
{
    const char input[] = "\n\n";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_junk)
{
    const char input[] = "n true";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_error);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_error);
}

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null)
{
    const char input[] = "null";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_null)
{
    const char input[] = "  null  ";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_null);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_nul)
{
    const char input[] = "nul";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_nuller)
{
    const char input[] = "nuller";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_error);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    const char input[] = "false";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_false);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_false)
{
    const char input[] = "  false  ";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_false);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_fals)
{
    const char input[] = "fals";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_falser)
{
    const char input[] = "falser";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_error);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    const char input[] = "true";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_true);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_white_true)
{
    const char input[] = "  true  ";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_true);
    decoder.next();
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_tru)
{
    const char input[] = "tru";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_eof);
}

BOOST_AUTO_TEST_CASE(test_fail_truer)
{
    const char input[] = "truer";
    json::decoder decoder(input, input + sizeof(input) - 1);
    BOOST_REQUIRE_EQUAL(decoder.type(), json::token_error);
}

BOOST_AUTO_TEST_SUITE_END()
