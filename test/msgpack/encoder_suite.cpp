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

#include <limits>
#include <protoc/msgpack/encoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(msgpack_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    char output[0];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    char output[1];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(output[0], '\xC0');
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    char output[0];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    char output[1];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(output[0], '\xC3');
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    char output[0];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    char output[1];
    msgpack::encoder encoder(output, output + sizeof(output));
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(output[0], '\xC2');
}

BOOST_AUTO_TEST_SUITE_END()
