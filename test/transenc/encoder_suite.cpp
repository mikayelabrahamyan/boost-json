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
#include <protoc/output.hpp>
#include <protoc/output_array.hpp>
#include <protoc/transenc/encoder.hpp>

using namespace protoc;

BOOST_AUTO_TEST_SUITE(transenc_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x82');
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x81');
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    output_array<0> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    output_array<1> buffer;
    transenc::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], '\x80');
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
