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

template<std::size_t N>
struct test_array : public output_array<char, N>
{
};

struct test_stream : public output_stream<char>
{
    test_stream(std::ostream& stream) : output_stream<char>(stream) {}
};

BOOST_AUTO_TEST_SUITE(json_encoder_suite)

//-----------------------------------------------------------------------------
// Basic
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_comma)
{
    test_array<1> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(std::string(buffer.begin(), buffer.size()), ",");
}

BOOST_AUTO_TEST_CASE(test_fail_comma)
{
    test_array<0> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 0);
}

BOOST_AUTO_TEST_CASE(test_colon)
{
    test_array<1> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_colon(), 1);
    BOOST_REQUIRE_EQUAL(std::string(buffer.begin(), buffer.size()), ":");
}

BOOST_AUTO_TEST_CASE(test_fail_colon)
{
    test_array<0> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_colon(), 0);
}

BOOST_AUTO_TEST_CASE(test_null)
{
    test_array<4> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(std::string(buffer.begin(), buffer.size()), "null");
}

BOOST_AUTO_TEST_CASE(test_fail_null)
{
    test_array<3> buffer;
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 0);
}

BOOST_AUTO_TEST_CASE(test_null_null)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null,null");
}

BOOST_AUTO_TEST_CASE(test_true)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true");
}

BOOST_AUTO_TEST_CASE(test_true_true)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "true,true");
}

BOOST_AUTO_TEST_CASE(test_false)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false");
}

BOOST_AUTO_TEST_CASE(test_false_false)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(result.str().data(), "false,false");
}

//-----------------------------------------------------------------------------
// Integer
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_int_zero_zero)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(0)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0,0");
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(1)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-1)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-1");
}

BOOST_AUTO_TEST_CASE(test_int_nine)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(9)), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "9");
}

BOOST_AUTO_TEST_CASE(test_int_minus_nine)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-9)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-9");
}

BOOST_AUTO_TEST_CASE(test_int_ten)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(10)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "10");
}

BOOST_AUTO_TEST_CASE(test_int_minus_ten)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-10)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-10");
}

BOOST_AUTO_TEST_CASE(test_int_almost_hundred)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(99)), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "99");
}

BOOST_AUTO_TEST_CASE(test_int_hundred)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(100)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "100");
}

BOOST_AUTO_TEST_CASE(test_int_almost_minus_hundred)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-99)), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-99");
}

BOOST_AUTO_TEST_CASE(test_int_minus_hundred)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(-100)), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-100");
}

BOOST_AUTO_TEST_CASE(test_int_max)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(std::numeric_limits<protoc::int64_t>::max())), 19);
    BOOST_REQUIRE_EQUAL(result.str().data(), "9223372036854775807");
}

BOOST_AUTO_TEST_CASE(test_int_almost_min)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(protoc::int64_t(std::numeric_limits<protoc::int64_t>::min() + 1)), 20);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-9223372036854775807");
}

BOOST_AUTO_TEST_CASE(test_int_min)
{
    std::ostringstream result;
    test_stream buffer(result);
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
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.0), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0");
}

BOOST_AUTO_TEST_CASE(test_float_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1.0), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1");
}

BOOST_AUTO_TEST_CASE(test_float_minus_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-1.0), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-1");
}

BOOST_AUTO_TEST_CASE(test_float_dot_five)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(0.5), 3);
    BOOST_REQUIRE_EQUAL(result.str().data(), "0.5");
}

BOOST_AUTO_TEST_CASE(test_float_minus_dot_five)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-0.5), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "-0.5");
}

BOOST_AUTO_TEST_CASE(test_float_e_100)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1e100), 6);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1e+100");
}

BOOST_AUTO_TEST_CASE(test_float_e_minus_100)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(1e-100), 6);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1e-100");
}

BOOST_AUTO_TEST_CASE(test_float_max)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::max()), 23);
    BOOST_REQUIRE_EQUAL(result.str().data(), "1.7976931348623157e+308");
}

BOOST_AUTO_TEST_CASE(test_float_min)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::min()), 23);
    BOOST_REQUIRE_EQUAL(result.str().data(), "2.2250738585072014e-308");
}

BOOST_AUTO_TEST_CASE(test_float_infinity)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::infinity()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_float_minus_infinity)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(-std::numeric_limits<protoc::float64_t>::infinity()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

BOOST_AUTO_TEST_CASE(test_float_nan)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(std::numeric_limits<protoc::float64_t>::quiet_NaN()), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "null");
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put(""), 2);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\"");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("alpha"), 7);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"alpha\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_quote)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\""), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\\"\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_reverse_solidus)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\\"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\\\\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_solidus)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("/"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\/\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_backspace)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\b"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\b\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_formfeed)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\f"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\f\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_newline)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\n"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\n\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_carriage_return)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\r"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\r\"");
}

BOOST_AUTO_TEST_CASE(test_string_escape_tab)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put("\t"), 4);
    BOOST_REQUIRE_EQUAL(result.str().data(), "\"\\t\"");
}

//-----------------------------------------------------------------------------
// Container
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_array_begin)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "[");
}

BOOST_AUTO_TEST_CASE(test_array_end)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "]");
}

BOOST_AUTO_TEST_CASE(test_array_empty)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "[]");
}

BOOST_AUTO_TEST_CASE(test_array_bool_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "[false]");
}

BOOST_AUTO_TEST_CASE(test_array_bool_two)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_array_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(encoder.put_array_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "[false,true]");
}

BOOST_AUTO_TEST_CASE(test_object_begin)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "{");
}

BOOST_AUTO_TEST_CASE(test_object_end)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "}");
}

BOOST_AUTO_TEST_CASE(test_object_empty)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "{}");
}

BOOST_AUTO_TEST_CASE(test_object_bool_one)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put("false"), 7);
    BOOST_REQUIRE_EQUAL(encoder.put_colon(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "{\"false\":false}");
}

BOOST_AUTO_TEST_CASE(test_object_bool_two)
{
    std::ostringstream result;
    test_stream buffer(result);
    json::encoder encoder(buffer);
    BOOST_REQUIRE_EQUAL(encoder.put_object_begin(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put("false"), 7);
    BOOST_REQUIRE_EQUAL(encoder.put_colon(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(false), 5);
    BOOST_REQUIRE_EQUAL(encoder.put_comma(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put("true"), 6);
    BOOST_REQUIRE_EQUAL(encoder.put_colon(), 1);
    BOOST_REQUIRE_EQUAL(encoder.put(true), 4);
    BOOST_REQUIRE_EQUAL(encoder.put_object_end(), 1);
    BOOST_REQUIRE_EQUAL(result.str().data(), "{\"false\":false,\"true\":true}");
}

BOOST_AUTO_TEST_SUITE_END()
