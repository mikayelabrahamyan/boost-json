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

#include <sstream>
#include <limits>
#include <protoc/output_array.hpp>
#include <protoc/output_stream.hpp>
#include <protoc/json/encoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(json_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null)
{
    output_array<4> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(std::string(buffer.begin(), buffer.size()), "null");
}

BOOST_AUTO_TEST_CASE(test_fail_null)
{
    output_array<3> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
}

BOOST_AUTO_TEST_CASE(test_null_null)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null null");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

BOOST_AUTO_TEST_CASE(test_true_true)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true true");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_false_false)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false false");
}

BOOST_AUTO_TEST_SUITE_END()
