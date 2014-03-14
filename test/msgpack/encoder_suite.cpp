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
#include <protoc/output_vector.hpp>
#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/detail/encoder.hpp>

namespace format = protoc::msgpack::detail;

template<std::size_t N>
struct test_array : public protoc::output_array<unsigned char, N>
{
};

struct test_vector : public protoc::output_vector<unsigned char>
{
};

BOOST_AUTO_TEST_SUITE(msgpack_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_null_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_null);
}

BOOST_AUTO_TEST_CASE(test_true_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_true);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_false_empty)
{
    test_array<0> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 0);
    BOOST_REQUIRE_EQUAL(buffer.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_false)
{
    test_array<1> buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_false);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_vector_null)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_null);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int8_zero)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(0)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x00);
}

BOOST_AUTO_TEST_CASE(test_int8_one)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0x01);
}

BOOST_AUTO_TEST_CASE(test_int8_minus_one)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-1)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xFF);
}

BOOST_AUTO_TEST_CASE(test_int8_max_minus)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-32)), 1);
    BOOST_REQUIRE_EQUAL(buffer.size(), 1);
    BOOST_REQUIRE_EQUAL(buffer[0], 0xE0);
}

BOOST_AUTO_TEST_CASE(test_int8_below_max_minus)
{
    test_vector buffer;
    format::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int8_t(-33)), 2);
    BOOST_REQUIRE_EQUAL(buffer.size(), 2);
    BOOST_REQUIRE_EQUAL(buffer[0], format::code_int8);
    BOOST_REQUIRE_EQUAL(buffer[1], 0xDF);
}

BOOST_AUTO_TEST_SUITE_END()
