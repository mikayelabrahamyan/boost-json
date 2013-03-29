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

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_int_zero_zero)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0 0");
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-1");
}

BOOST_AUTO_TEST_CASE(test_int_nine)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(9)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "9");
}

BOOST_AUTO_TEST_CASE(test_int_minus_nine)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-9)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-9");
}

BOOST_AUTO_TEST_CASE(test_int_ten)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(10)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "10");
}

BOOST_AUTO_TEST_CASE(test_int_minus_ten)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-10)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-10");
}

BOOST_AUTO_TEST_CASE(test_int_almost_hundred)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(99)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "99");
}

BOOST_AUTO_TEST_CASE(test_int_hundred)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(100)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "100");
}

BOOST_AUTO_TEST_CASE(test_int_almost_minus_hundred)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-99)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-99");
}

BOOST_AUTO_TEST_CASE(test_int_minus_hundred)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-100)), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-100");
}

BOOST_AUTO_TEST_CASE(test_int_max)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(std::numeric_limits<protoc::int64_t>::max())), 19);
    BOOST_REQUIRE_EQUAL(result.str().data(), "9223372036854775807");
}

BOOST_AUTO_TEST_CASE(test_int_almost_min)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(std::numeric_limits<protoc::int64_t>::min() + 1)), 20);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-9223372036854775807");
}

BOOST_AUTO_TEST_CASE(test_int_min)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(std::numeric_limits<protoc::int64_t>::min())), 20);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-9223372036854775808");
}

//-----------------------------------------------------------------------------
// Float
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_zero)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-1");
}

BOOST_AUTO_TEST_CASE(test_float_dot_five)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.5), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0.5");
}

BOOST_AUTO_TEST_CASE(test_float_minus_dot_five)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-0.5), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-0.5");
}

BOOST_AUTO_TEST_CASE(test_float_e_100)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1e100), 6);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1e+100");
}

BOOST_AUTO_TEST_CASE(test_float_e_minus_100)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1e-100), 6);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1e-100");
}

BOOST_AUTO_TEST_CASE(test_float_max)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::max()), 23);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1.7976931348623157e+308");
}

BOOST_AUTO_TEST_CASE(test_float_min)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::min()), 23);
    BOOST_REQUIRE_EQUAL(result.str().data(), "2.2250738585072014e-308");
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    std::ostringstream result;
    output_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_SUITE_END()
