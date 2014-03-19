///////////////////////////////////////////////////////////////////////////////
//
// http://protoc.sourceforge.net/
//
// Copyright (C) 2014 Bjorn Reese <breese@users.sourceforge.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>

#include <protoc/exceptions.hpp>
#include <protoc/msgpack/detail/codes.hpp>
#include <protoc/msgpack/iarchive.hpp>
#include <protoc/msgpack/string.hpp>

namespace format = protoc::msgpack;
namespace detail = protoc::msgpack::detail;

BOOST_AUTO_TEST_SUITE(msgpack_iarchive_suite)

//-----------------------------------------------------------------------------
// Basic types
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_false)
{
    format::iarchive::value_type input[] = { detail::code_false };
    format::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, false);
}

BOOST_AUTO_TEST_CASE(test_true)
{
    format::iarchive::value_type input[] = { detail::code_true };
    format::iarchive in(input, input + sizeof(input));
    bool value = false;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, true);
}

BOOST_AUTO_TEST_CASE(test_bool_junk)
{
    format::iarchive::value_type input[] = { detail::code_null }; // Null cannot be deserialized as bool (only as optional<bool>)
    format::iarchive in(input, input + sizeof(input));
    bool value = true;
    BOOST_REQUIRE_THROW(in >> value,
                        protoc::invalid_value);
}

//-----------------------------------------------------------------------------
// Integers
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_int_zero)
{
    format::iarchive::value_type input[] = { 0x00 };
    format::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 0);
}

BOOST_AUTO_TEST_CASE(test_int_one)
{
    format::iarchive::value_type input[] = { 0x01 };
    format::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1);
}

BOOST_AUTO_TEST_CASE(test_int_minus_one)
{
    format::iarchive::value_type input[] = { 0xFF };
    format::iarchive in(input, input + sizeof(input));
    int value = 99;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, -1);
}

//-----------------------------------------------------------------------------
// Floating-point
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_float_one)
{
    format::iarchive::value_type input[] = { detail::code_float32, 0x3F, 0x80, 0x00, 0x00 };
    format::iarchive in(input, input + sizeof(input));
    protoc::float32_t value = 0.0f;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1.0f);
}

BOOST_AUTO_TEST_CASE(test_double_one)
{
    format::iarchive::value_type input[] = { detail::code_float64, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    format::iarchive in(input, input + sizeof(input));
    protoc::float64_t value = 0.0;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, 1.0);
}

//-----------------------------------------------------------------------------
// String
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(test_string_empty)
{
    format::iarchive::value_type input[] = { detail::code_str8, 0x00 };
    format::iarchive in(input, input + sizeof(input));
    std::string value("replace");
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "");
}

BOOST_AUTO_TEST_CASE(test_string_alpha)
{
    format::iarchive::value_type input[] = { detail::code_str8, 0x05, 'a', 'l', 'p', 'h', 'a' };
    format::iarchive in(input, input + sizeof(input));
    std::string value;
    BOOST_REQUIRE_NO_THROW(in >> value);
    BOOST_REQUIRE_EQUAL(value, "alpha");
}

BOOST_AUTO_TEST_SUITE_END()
